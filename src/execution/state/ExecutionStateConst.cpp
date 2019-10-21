/*
 * ExecutionStateConst.cpp
 *
 *  Created on: 19 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"

shared_ptr<DynVal> ExecutionState::evalConst(const Constant *cv) {
  //  errs() << "Evaluation::evalConst: ValueID = " << cv->getValueID() << "\n";
  //  cv->print(errs(), true);
  //  errs() << "\n";

  switch (cv->getValueID()) {
  case Value::ConstantIntVal: {
    auto cInt = cast<ConstantInt>(cv);
    return std::make_shared<IntVal>(cInt->getValue());
  }
  case Value::ConstantPointerNullVal: {
    return std::make_shared<PointerVal>(0, AddressSpace::GLOBAL);
  }
  case Value::ConstantDataArrayVal: {
    auto cArray = cast<ConstantDataArray>(cv);
    auto arraySize = cArray->getType()->getNumElements();
    shared_ptr<ArrayVal> res = std::make_shared<ArrayVal>(
        dataLayout->getTypeAllocSize(cArray->getType()->getElementType()),
        arraySize);
    for (unsigned i = 0; i < arraySize; ++i) {
      res->array.push_back(evalConst(cArray->getElementAsConstant(i)));
    }
    return std::move(res);
  }
  case Value::ConstantAggregateZeroVal: {
    auto caz = cast<ConstantAggregateZero>(cv);
    auto type = cv->getType();
    if (type->isStructTy()) {
      auto stType = cast<StructType>(type);
      auto stLayout = dataLayout->getStructLayout(stType);
      shared_ptr<StructVal> res =
          std::make_shared<StructVal>(dataLayout->getTypeAllocSize(stType));
      for (auto i = 0u; i < stType->getNumElements(); ++i) {
        auto offset = stLayout->getElementOffset(i);
        res->addField(offset, evalConst(caz->getElementValue(i)));
      }
      return std::move(res);
    } else if (type->isArrayTy()) {
      auto arrayType = cast<ArrayType>(type);
      auto arraySize = arrayType->getNumElements();
      shared_ptr<ArrayVal> res = std::make_shared<ArrayVal>(
          dataLayout->getTypeAllocSize(arrayType->getElementType()), arraySize);
      for (unsigned i = 0; i < arraySize; ++i) {
        res->array.push_back(evalConst(caz->getSequentialElement()));
      }
      return std::move(res);
    } else {
      errs() << "SymExecutor::evalConst: Unknown "
                "Value::ConstantAggregateZeroVal "
             << cv->getValueID() << "\n";
      break;
    }
  }
  case Value::ConstantExprVal: {
    auto cExpr = cast<ConstantExpr>(cv);
    return evalConstExpr(cExpr);
  }
  case Value::GlobalVariableVal: {
    auto glbVar = cast<GlobalValue>(cv);
    auto globalAddr = globalAddresses.at(glbVar);
    return std::make_shared<PointerVal>(globalAddr, AddressSpace::GLOBAL);
  }
  case Value::GlobalAliasVal: {
    auto glbAlias = cast<GlobalAlias>(cv);
    return evalConst(glbAlias->getAliasee());
  }
  case Value::ConstantFPVal: {
    auto cFloat = cast<ConstantFP>(cv);
    return std::make_shared<FloatVal>(cFloat->getValueAPF().convertToDouble(),
                                      cFloat->getType()->isDoubleTy());
  }
  case Value::ConstantArrayVal: {
    auto cArray = cast<ConstantArray>(cv);
    auto arraySize = cArray->getType()->getNumElements();
    shared_ptr<ArrayVal> res = std::make_shared<ArrayVal>(
        dataLayout->getTypeAllocSize(cArray->getType()->getElementType()),
        arraySize);
    for (unsigned i = 0; i < arraySize; ++i) {
      res->array.push_back(evalConst(cArray->getOperand(i)));
    }
    return std::move(res);
  }
  case Value::ConstantStructVal: {
    auto cStruct = cast<ConstantStruct>(cv);
    auto stType = cStruct->getType();
    auto stSize = stType->getNumElements();
    auto stLayout = dataLayout->getStructLayout(stType);
    shared_ptr<StructVal> res =
        std::make_shared<StructVal>(dataLayout->getTypeAllocSize(stType));
    for (auto i = 0u; i < stType->getNumElements(); ++i) {
      auto offset = stLayout->getElementOffset(i);
      res->addField(offset, evalConst(cStruct->getOperand(i)));
    }
    return std::move(res);
  }
  case Value::FunctionVal: {
    auto fun = cast<Function>(cv);
    auto funAddr = globalAddresses.at(fun);
    return std::make_shared<PointerVal>(funAddr, AddressSpace::GLOBAL);
  }
  case Value::UndefValueVal: {
    auto type = cv->getType();
    if (type->isStructTy()) {
      auto stType = cast<StructType>(type);
      auto stLayout = dataLayout->getStructLayout(stType);
      auto retVal =
          std::make_shared<StructVal>(dataLayout->getTypeAllocSize(stType));

      for (auto i = 0u, e = stType->getNumElements(); i < e; ++i) {
        auto elemType = stType->getElementType(i);
        auto offset = stLayout->getElementOffset(i);
        if (!elemType->isAggregateType())
          retVal->addField(offset,
                           std::make_shared<DynVal>(DynValType::UNDEF_VAL));
        else
          retVal->addField(offset, evalConst(UndefValue::get(elemType)));
      }
      return std::move(retVal);
    } else if (type->isArrayTy()) {
      auto arrayType = cast<ArrayType>(type);
      auto elemType = arrayType->getElementType();
      auto arraySize = arrayType->getNumElements();
      auto retVal = std::make_shared<ArrayVal>(
          dataLayout->getTypeAllocSize(elemType), arraySize);
      for (unsigned i = 0; i < arraySize; ++i) {
        if (elemType->isAggregateType())
          retVal->array.push_back(evalConst(UndefValue::get(elemType)));
      }
      return std::move(retVal);
    } else if (type->isVectorTy())
      llvm_unreachable("Vector type not supported");
    else
      return std::make_shared<DynVal>(DynValType::ERROR);
    break;
  }
  default: {
    errs() << "SymExecutor::evalConst: not supported constant: "
           << cv->getValueID() << "\n";
    cv->print(errs(), true);
  }
  }
  return std::make_shared<DynVal>(DynValType::ERROR);
}

shared_ptr<DynVal> ExecutionState::evalConstExpr(const ConstantExpr *cexpr) {
  //  errs() << "SymExecutor::evalConstExpr: Opcode = " <<
  //  cexpr->getOpcode()
  //         << "\n";
  //  cexpr->print(errs(), true);
  //  errs() << "\n";

  // Local helper function to avoid code duplication
  auto evalConstIntUnOp = [this, cexpr](auto unOp) -> shared_ptr<DynVal> {
    auto srcVal = this->evalConst(cexpr->getOperand(0));
    shared_ptr<IntVal> tmp = std::static_pointer_cast<IntVal>(srcVal);
    shared_ptr<DynVal> res = std::make_shared<IntVal>(unOp(tmp->intVal));
    return res;
  };
  auto evalConstIntBinOp = [this, cexpr](auto binOp) -> shared_ptr<DynVal> {
    auto srcVal0 = this->evalConst(cexpr->getOperand(0));
    shared_ptr<IntVal> tmp0 = std::static_pointer_cast<IntVal>(srcVal0);
    auto srcVal1 = this->evalConst(cexpr->getOperand(1));
    shared_ptr<IntVal> tmp1 = std::static_pointer_cast<IntVal>(srcVal1);
    shared_ptr<DynVal> res =
        std::make_shared<IntVal>(binOp(tmp0->intVal, tmp1->intVal));
    return res;
  };

  auto evalConstFloatBinOp = [this, cexpr](auto binOp) -> shared_ptr<DynVal> {
    auto srcVal0 = this->evalConst(cexpr->getOperand(0));
    shared_ptr<FloatVal> tmp0 = std::static_pointer_cast<FloatVal>(srcVal0);
    auto srcVal1 = this->evalConst(cexpr->getOperand(1));
    shared_ptr<FloatVal> tmp1 = std::static_pointer_cast<FloatVal>(srcVal1);
    shared_ptr<DynVal> res = std::make_shared<FloatVal>(
        binOp(tmp0->fpVal, tmp1->fpVal), tmp0->isDouble || tmp1->isDouble);
    return res;
  };

  switch (cexpr->getOpcode()) {
  case Instruction::Trunc: {
    auto intWidth = cast<IntegerType>(cexpr->getType())->getBitWidth();
    return evalConstIntUnOp(
        [intWidth](const APInt &intVal) { return intVal.trunc(intWidth); });
  }
  case Instruction::ZExt: {
    auto intWidth = cast<IntegerType>(cexpr->getType())->getBitWidth();
    return evalConstIntUnOp(
        [intWidth](const APInt &intVal) { return intVal.zext(intWidth); });
  }
  case Instruction::SExt: {
    auto intWidth = cast<IntegerType>(cexpr->getType())->getBitWidth();
    return evalConstIntUnOp(
        [intWidth](const APInt &intVal) { return intVal.sext(intWidth); });
  }
  case Instruction::FPTrunc: {
    auto srcType = cexpr->getOperand(0)->getType();
    auto dstType = cexpr->getType();
    if (!(srcType->isDoubleTy() && dstType->isFloatTy()))
      llvm_unreachable("Invalid FPTrunc instruction");

    auto srcVal = evalConst(cexpr->getOperand(0));
    auto srcFloatVal = std::static_pointer_cast<FloatVal>(srcVal);
    assert(srcFloatVal->isDouble);
    return std::make_shared<FloatVal>(static_cast<float>(srcFloatVal->fpVal),
                                      false);
  }
  case Instruction::FPExt: {
    auto srcType = cexpr->getOperand(0)->getType();
    auto dstType = cexpr->getType();
    if (!(dstType->isDoubleTy() && srcType->isFloatTy()))
      llvm_unreachable("Invalid FPExt instruction");

    auto srcVal = evalConst(cexpr->getOperand(0));
    auto srcFloatVal = std::static_pointer_cast<FloatVal>(srcVal);
    assert(!srcFloatVal->isDouble);
    // FPExt is a noop in this implementation
    return std::make_shared<FloatVal>(srcFloatVal->fpVal, true);
  }
  case Instruction::UIToFP:
  case Instruction::SIToFP: {
    auto srcVal = evalConst(cexpr->getOperand(0));
    return std::make_shared<FloatVal>(
        APIntOps::RoundAPIntToDouble(
            std::static_pointer_cast<IntVal>(srcVal)->intVal),
        cexpr->getType()->isDoubleTy());
  }
  case Instruction::FPToUI:
  case Instruction::FPToSI: {
    auto intWidth = cast<IntegerType>(cexpr->getType())->getBitWidth();

    auto srcVal = evalConst(cexpr->getOperand(0));
    return std::make_shared<IntVal>(APIntOps::RoundDoubleToAPInt(
        std::static_pointer_cast<FloatVal>(srcVal)->fpVal, intWidth));
  }
  case Instruction::PtrToInt: {
    auto intWidth = cast<IntegerType>(cexpr->getType())->getBitWidth();
    auto srcVal =
        std::static_pointer_cast<PointerVal>(evalConst(cexpr->getOperand(0)))
            ->address;
    return std::make_shared<IntVal>(APInt(intWidth, srcVal));
  }
  case Instruction::IntToPtr: {
    auto srcVal = evalConst(cexpr->getOperand(0));
    auto ptrSize = dataLayout->getPointerSizeInBits();
    return std::make_shared<PointerVal>(std::static_pointer_cast<IntVal>(srcVal)
                                            ->intVal.zextOrTrunc(ptrSize)
                                            .getZExtValue(),
                                        AddressSpace::GLOBAL);
  }
  case Instruction::BitCast: {
    auto op0 = cexpr->getOperand(0);
    auto srcType = op0->getType();
    auto dstType = cexpr->getType();
    auto srcVal = evalConst(op0);

    if (dstType->isPointerTy()) {
      if (!srcType->isPointerTy())
        llvm_unreachable("Invalid Bitcast");
      return std::move(srcVal);
    } else if (dstType->isIntegerTy()) {
      if (srcType->isFloatTy() || srcType->isDoubleTy()) {
        return std::make_shared<IntVal>(APInt::doubleToBits(
            std::static_pointer_cast<FloatVal>(srcVal)->fpVal));
      } else if (srcType->isIntegerTy()) {
        return std::move(srcVal);
      } else {
        llvm_unreachable("Invalid BitCast");
      }
    } else if (dstType->isFloatTy() || dstType->isDoubleTy()) {
      if (srcType->isIntegerTy()) {
        return std::make_shared<FloatVal>(
            std::static_pointer_cast<IntVal>(srcVal)->intVal.bitsToDouble(),
            dstType->isDoubleTy());
      } else {
        return std::make_shared<FloatVal>(
            std::static_pointer_cast<FloatVal>(srcVal)->fpVal,
            dstType->isDoubleTy());
      }
    } else {
      llvm_unreachable("Invalid Bitcast");
    }
    break;
  }

  case Instruction::Add: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0 + i1; });
  }
  case Instruction::Sub: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0 - i1; });
  }
  case Instruction::Mul: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0 * i1; });
  }
  case Instruction::UDiv: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0.udiv(i1); });
  }
  case Instruction::SDiv: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0.sdiv(i1); });
  }
  case Instruction::URem: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0.urem(i1); });
  }
  case Instruction::SRem: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0.srem(i1); });
  }
  case Instruction::And: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0 & i1; });
  }
  case Instruction::Or: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0 | i1; });
  }
  case Instruction::Xor: {
    return evalConstIntBinOp(
        [](const APInt &i0, const APInt &i1) { return i0 ^ i1; });
  }
  case Instruction::Shl: {
    return evalConstIntBinOp([](const APInt &value, const APInt &shift) {
      auto shiftAmount = shift.getZExtValue();
      auto valueWidth = value.getBitWidth();
      if (shiftAmount > valueWidth)
        llvm_unreachable("Illegal shift amount");
      return value.shl(shiftAmount);
    });
  }
  case Instruction::LShr: {
    return evalConstIntBinOp([](const APInt &value, const APInt &shift) {
      auto shiftAmount = shift.getZExtValue();
      auto valueWidth = value.getBitWidth();
      if (shiftAmount > valueWidth)
        llvm_unreachable("Illegal shift amount");
      return value.lshr(shiftAmount);
    });
  }
  case Instruction::AShr: {
    return evalConstIntBinOp([](const APInt &value, const APInt &shift) {
      auto shiftAmount = shift.getZExtValue();
      auto valueWidth = value.getBitWidth();
      if (shiftAmount > valueWidth)
        llvm_unreachable("Illegal shift amount");
      return value.ashr(shiftAmount);
    });
  }
  case Instruction::ICmp: {
    auto cmp = [cexpr](const APInt &i0, const APInt &i1) {
      switch (cexpr->getPredicate()) {
      case CmpInst::ICMP_EQ:
        return APInt(1, i0 == i1);
      case CmpInst::ICMP_NE:
        return APInt(1, i0 != i1);
      case CmpInst::ICMP_UGT:
        return APInt(1, i0.ugt(i1));
      case CmpInst::ICMP_UGE:
        return APInt(1, i0.uge(i1));
      case CmpInst::ICMP_ULT:
        return APInt(1, i0.ult(i1));
      case CmpInst::ICMP_ULE:
        return APInt(1, i0.ule(i1));
      case CmpInst::ICMP_SGT:
        return APInt(1, i0.sgt(i1));
      case CmpInst::ICMP_SGE:
        return APInt(1, i0.sge(i1));
      case CmpInst::ICMP_SLT:
        return APInt(1, i0.slt(i1));
      case CmpInst::ICMP_SLE:
        return APInt(1, i0.sle(i1));
      default:
        llvm_unreachable("Illegal icmp predicate");
      }
    };

    // ICmp can compare both integers and pointers, so we cannot just use
    // evaluateIntBinOp
    auto val0 = evalConst(cexpr->getOperand(0));
    auto val1 = evalConst(cexpr->getOperand(1));
    if (val0->valType == DynValType::INT_VAL &&
        val1->valType == DynValType::INT_VAL) {
      auto res = cmp(std::static_pointer_cast<IntVal>(val0)->intVal,
                     std::static_pointer_cast<IntVal>(val1)->intVal);
      return std::make_shared<IntVal>(res);
    } else if (val0->valType == DynValType::POINTER_VAL &&
               val1->valType == DynValType::POINTER_VAL) {
      auto ptrSize = dataLayout->getPointerSizeInBits();
      auto addr0 = std::static_pointer_cast<PointerVal>(val0)->address;
      auto addr1 = std::static_pointer_cast<PointerVal>(val1)->address;
      auto res = cmp(APInt(ptrSize, addr0), APInt(ptrSize, addr1));
      return std::make_shared<IntVal>(res);
    } else {
      llvm_unreachable("Illegal icmp compare types");
    }
    break;
  }
  case Instruction::FAdd: {
    return evalConstFloatBinOp([](double f0, double f1) { return f0 + f1; });
  }
  case Instruction::FSub: {
    return evalConstFloatBinOp([](double f0, double f1) { return f0 - f1; });
  }
  case Instruction::FMul: {
    return evalConstFloatBinOp([](double f0, double f1) { return f0 * f1; });
  }
  case Instruction::FDiv: {
    return evalConstFloatBinOp([](double f0, double f1) { return f0 / f1; });
  }
  case Instruction::FRem: {
    return evalConstFloatBinOp(
        [](double f0, double f1) { return std::remainder(f0, f1); });
  }
  case Instruction::FCmp: {
    auto srcVal0 = evalConst(cexpr->getOperand(0));
    auto srcVal1 = evalConst(cexpr->getOperand(1));

    auto f0 = std::static_pointer_cast<FloatVal>(srcVal0)->fpVal;
    auto f1 = std::static_pointer_cast<FloatVal>(srcVal1)->fpVal;
    auto isF0Nan = std::isnan(f0);
    auto isF1Nan = std::isnan(f1);
    auto bothNotNan = !isF0Nan && !isF1Nan;
    auto eitherIsNan = isF0Nan || isF1Nan;

    switch (cexpr->getPredicate()) {
    case CmpInst::FCMP_FALSE:
      return std::make_shared<IntVal>(APInt(1, false));
    case CmpInst::FCMP_OEQ:
      return std::make_shared<IntVal>(APInt(1, bothNotNan && f0 == f1));
    case CmpInst::FCMP_OGT:
      return std::make_shared<IntVal>(APInt(1, bothNotNan && f0 > f1));
    case CmpInst::FCMP_OGE:
      return std::make_shared<IntVal>(APInt(1, bothNotNan && f0 >= f1));
    case CmpInst::FCMP_OLT:
      return std::make_shared<IntVal>(APInt(1, bothNotNan && f0 < f1));
    case CmpInst::FCMP_OLE:
      return std::make_shared<IntVal>(APInt(1, bothNotNan && f0 <= f1));
    case CmpInst::FCMP_ONE:
      return std::make_shared<IntVal>(APInt(1, bothNotNan && f0 != f1));
    case CmpInst::FCMP_ORD:
      return std::make_shared<IntVal>(APInt(1, bothNotNan));
    case CmpInst::FCMP_UEQ:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan || f0 == f1));
    case CmpInst::FCMP_UGT:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan || f0 > f1));
    case CmpInst::FCMP_UGE:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan || f0 >= f1));
    case CmpInst::FCMP_ULT:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan || f0 < f1));
    case CmpInst::FCMP_ULE:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan || f0 <= f1));
    case CmpInst::FCMP_UNE:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan || f0 != f1));
    case CmpInst::FCMP_UNO:
      return std::make_shared<IntVal>(APInt(1, eitherIsNan));
    case CmpInst::FCMP_TRUE:
      return std::make_shared<IntVal>(APInt(1, true));
    default: {
      llvm_unreachable("Illegal fcmp predicate");
    }
    }
    break;
  }
  case Instruction::Select: {
    auto condVal = evalConst(cexpr->getOperand(0));
    auto condInt =
        std::static_pointer_cast<IntVal>(condVal)->intVal.getBoolValue();
    if (condInt)
      return evalConst(cexpr->getOperand(1));
    else
      return evalConst(cexpr->getOperand(2));
  }
  case Instruction::GetElementPtr: {
    auto baseVal = evalConst(cexpr->getOperand(0));
    auto basePtrVal = std::static_pointer_cast<PointerVal>(baseVal);
    auto offsetInt = APInt(dataLayout->getPointerSizeInBits(), 0);
    cast<GEPOperator>(cexpr)->accumulateConstantOffset(*dataLayout, offsetInt);
    return std::make_shared<PointerVal>(
        basePtrVal->address + offsetInt.getZExtValue(), basePtrVal->space);
  }
  case Instruction::ExtractValue: {
    auto baseVal = evalConst(cexpr->getOperand(0));
    for (auto i = 1u, e = cexpr->getNumOperands(); i < e; ++i) {
      auto idxVal = evalConst(cexpr->getOperand(i));
      auto seqNum =
          std::static_pointer_cast<IntVal>(idxVal)->intVal.getZExtValue();
      if (baseVal->valType == DynValType::STRUCT_VAL) {
        auto fieldVal =
            std::static_pointer_cast<StructVal>(baseVal)->getFieldAtOffset(
                seqNum);
        baseVal = std::move(fieldVal);
      } else if (baseVal->valType == DynValType::ARRAY_VAL) {
        auto elemVal =
            std::static_pointer_cast<ArrayVal>(baseVal)->getElementAtIndex(
                seqNum);
        baseVal = std::move(elemVal);
      } else
        llvm_unreachable("extractvalue into a non-aggregate value!");
    }
    return baseVal;
  }
  case Instruction::InsertValue: {
    auto baseVal = evalConst(cexpr->getOperand(0));
    auto &tmpBaseVal = baseVal;
    for (auto i = 2u, e = cexpr->getNumOperands(); i < e; ++i) {
      auto idxVal = evalConst(cexpr->getOperand(i));
      auto seqNum =
          std::static_pointer_cast<IntVal>(idxVal)->intVal.getZExtValue();
      if (baseVal->valType == DynValType::STRUCT_VAL) {
        auto fieldVal =
            std::static_pointer_cast<StructVal>(baseVal)->getFieldAtOffset(
                seqNum);
        baseVal = std::move(fieldVal);
      } else if (baseVal->valType == DynValType::ARRAY_VAL) {
        auto elemVal =
            std::static_pointer_cast<ArrayVal>(baseVal)->getElementAtIndex(
                seqNum);
        baseVal = std::move(elemVal);
      } else
        llvm_unreachable("insertvalue into a non-aggregate value!");
    }
    baseVal = evalConst(cexpr->getOperand(1));

    return tmpBaseVal;
  }
  case Instruction::InsertElement:
  case Instruction::ExtractElement:
  case Instruction::ShuffleVector:
  case Instruction::AddrSpaceCast:
  default: {
    errs() << "SymExecutor::evalConstExpr: not supported constant "
              "expression: "
           << cexpr->getOpcode() << "\n";
    cexpr->print(errs(), true);
  }
  }
  return std::make_shared<DynVal>(DynValType::ERROR);
}
