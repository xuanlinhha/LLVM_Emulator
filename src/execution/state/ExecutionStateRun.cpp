/*
 * ExecutionStateRun.cpp
 *
 *  Created on: 19 Oct 2019
 *      Author: xuanlinhha
 */

#include "ExecutionState.h"

void ExecutionState::executeNonTerminator() {
  //  currentInst->print(errs(), true);
  //  errs() << " -- executeNonTerminator\n";
  auto evaluateFloatBinOp = [this](auto binOp) {
    auto val0 = evalOperand(currentInst->getOperand(0));
    FloatVal *tmp0 = (FloatVal *)val0;
    auto val1 = evalOperand(currentInst->getOperand(1));
    FloatVal *tmp1 = (FloatVal *)val1;
    assert(tmp0->isDouble == tmp1->isDouble);
    frames.back()->insertBinding(
        currentInst,
        new FloatVal(binOp(tmp0->fpVal, tmp1->fpVal), tmp0->isDouble));
  };

  const Instruction *prevInst = currentInst;

  //  prevInst->print(errs(), true);
  //  errs() << "\n";

  switch (currentInst->getOpcode()) {
    // Memory instructions...
  case Instruction::Alloca: {
    auto allocInst = cast<AllocaInst>(currentInst);
    auto allocElems = 1u;
    if (allocInst->isArrayAllocation()) {
      auto sizeVal = evalOperand(allocInst->getArraySize());
      allocElems = ((IntVal *)sizeVal)->intVal.getZExtValue();
    }
    auto allocSize = allocElems * dataLayout->getTypeAllocSize(
                                      allocInst->getType()->getElementType());
    auto retAddr = allocateStackMem(allocSize);
    frames.back()->insertBinding(currentInst,
                                 new PointerVal(retAddr, AddressSpace::STACK));
    break;
  }
  case Instruction::Store: {
    auto storeInst = cast<StoreInst>(currentInst);
    auto storeSrc = evalOperand(storeInst->getPointerOperand());
    auto storePtr = (PointerVal *)storeSrc;
    auto storeVal = evalOperand(storeInst->getValueOperand());
    //    storeVal->print();
    //    errs() << "\n";
    writeToPointer(storePtr, storeVal);
    break;
  }
  case Instruction::Load: {
    auto loadInst = cast<LoadInst>(currentInst);
    auto loadType = loadInst->getType();
    auto loadSrc = evalOperand(loadInst->getPointerOperand());
    auto loadPtr = (PointerVal *)loadSrc;
    auto resVal = readFromPointer(loadPtr, loadType);
    frames.back()->insertBinding(currentInst, resVal);
    break;
  }
  case Instruction::GetElementPtr: {
    auto gepInst = cast<GetElementPtrInst>(currentInst);
    auto baseVal = evalOperand(gepInst->getPointerOperand());
    auto basePtrVal = (PointerVal *)baseVal;
    auto baseAddr = basePtrVal->address;
    for (auto itr = gep_type_begin(gepInst), ite = gep_type_end(gepInst);
         itr != ite; ++itr) {
      auto idxVal = evalOperand(itr.getOperand());
      auto seqNum = ((IntVal *)idxVal)->intVal.getSExtValue();
      if (itr.isStruct()) {
        baseAddr += dataLayout->getStructLayout(itr.getStructType())
                        ->getElementOffset(seqNum);
      } else {
        baseAddr += seqNum * dataLayout->getTypeAllocSize(itr.getIndexedType());
      }
    }
    PointerVal *p = new PointerVal(baseAddr, basePtrVal->space);
    frames.back()->insertBinding(currentInst, std::move(p));
    break;
  }

  // arithmetic
  case Instruction::Add: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::ADD, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal + op1->intVal));
    }
    break;
  }
  case Instruction::Sub: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::SUB, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      //      errs() << "Begin Instruction::Sub\n";
      //      val0->print();
      //      errs() << "\n";
      //      val1->print();
      //      errs() << "\n";
      //      tmp->print();
      //      errs() << "\n";
      //      errs() << "End Instruction::Sub\n";
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal - op1->intVal));
    }
    break;
  }
  case Instruction::Mul: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::MUL, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal * op1->intVal));
    }
    break;
  }
  case Instruction::UDiv: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::UDIV, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.udiv(op1->intVal)));
    }
    break;
  }
  case Instruction::SDiv: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::SDIV, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.sdiv(op1->intVal)));
    }
    break;
  }
  case Instruction::URem: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::UREM, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.urem(op1->intVal)));
    }
    break;
  }
  case Instruction::SRem: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::SREM, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.srem(op1->intVal)));
    }
    break;
  }
  // float arithmetic
  case Instruction::FAdd: {
    evaluateFloatBinOp([](double f0, double f1) { return f0 + f1; });
    break;
  }
  case Instruction::FSub: {
    evaluateFloatBinOp([](double f0, double f1) { return f0 - f1; });
    break;
  }

  case Instruction::FMul: {
    evaluateFloatBinOp([](double f0, double f1) { return f0 * f1; });
    break;
  }
  case Instruction::FDiv: {
    evaluateFloatBinOp([](double f0, double f1) { return f0 / f1; });
    break;
  }
  case Instruction::FRem: {
    evaluateFloatBinOp(
        [](double f0, double f1) { return std::remainder(f0, f1); });
    break;
  }
  // Logical operators...
  case Instruction::And: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op1->bitWidth, SymExprType::AND, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, tmp);
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal & op1->intVal));
    }
    break;
  }
  case Instruction::Or: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::OR, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal | op1->intVal));
    }
    break;
  }
  case Instruction::Xor: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    if (op0->isSym || op1->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::XOR, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal ^ op1->intVal));
    }
    break;
  }
  case Instruction::Shl: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    assert(!op1->isSym && "Cannot handle Shl with a symbolic length!");
    if (op0->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::SHL, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      assert((op0->intVal.getBitWidth() >= op1->intVal.getZExtValue()) &&
             "Invalid Shl amount!");
      frames.back()->insertBinding(
          currentInst, new IntVal(op0->intVal.shl(op1->intVal.getZExtValue())));
    }
    break;
  }
  case Instruction::LShr: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    assert(!op1->isSym && "Cannot handle LShr with a symbolic length!");
    if (op0->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::LSHR, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      assert((op0->intVal.getBitWidth() >= op1->intVal.getZExtValue()) &&
             "Invalid LShr amount!");
      frames.back()->insertBinding(
          currentInst,
          new IntVal(op0->intVal.lshr(op1->intVal.getZExtValue())));
    }
    break;
  }
  case Instruction::AShr: {
    IntVal *op0 = (IntVal *)evalOperand(currentInst->getOperand(0));
    IntVal *op1 = (IntVal *)evalOperand(currentInst->getOperand(1));
    assert(!op1->isSym && "Cannot handle AShr with a symbolic length!");
    if (op0->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::ASHR, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(op1);
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      assert((op0->intVal.getBitWidth() >= op1->intVal.getZExtValue()) &&
             "Invalid AShr amount!");
      frames.back()->insertBinding(
          currentInst,
          new IntVal(op0->intVal.ashr(op1->intVal.getZExtValue())));
    }
    break;
  }

  // compare
  case Instruction::ICmp: {
    auto cmp = [this](const APInt &i0, const APInt &i1) {
      switch (cast<CmpInst>(currentInst)->getPredicate()) {
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

    auto symCmp = [this](SimVal *v0, SimVal *v1) {
      IntVal *res;
      switch (cast<CmpInst>(currentInst)->getPredicate()) {
      case CmpInst::ICMP_EQ: {
        res = new IntVal(1, SymExprType::EQ, "");
        break;
      }
      case CmpInst::ICMP_NE: {
        res = new IntVal(1, SymExprType::NE, "");
        break;
      }
      case CmpInst::ICMP_UGT: {
        res = new IntVal(1, SymExprType::UGT, "");
        break;
      }
      case CmpInst::ICMP_UGE: {
        res = new IntVal(1, SymExprType::UGE, "");
        break;
      }
      case CmpInst::ICMP_ULT: {
        res = new IntVal(1, SymExprType::ULT, "");
        break;
      }
      case CmpInst::ICMP_ULE: {
        res = new IntVal(1, SymExprType::ULE, "");
        break;
      }
      case CmpInst::ICMP_SGT: {
        res = new IntVal(1, SymExprType::SGT, "");
        break;
      }
      case CmpInst::ICMP_SGE: {
        res = new IntVal(1, SymExprType::SGE, "");
        break;
      }
      case CmpInst::ICMP_SLT: {
        res = new IntVal(1, SymExprType::SLT, "");
        break;
      }
      case CmpInst::ICMP_SLE: {
        res = new IntVal(1, SymExprType::SLE, "");
        break;
      }
      default: {
        res = new IntVal(1, SymExprType::INVALID, "");
      }
      }
      res->operands.push_back(v0);
      res->operands.push_back(v1);

      return res;
    };

    // ICmp can compare both integers and pointers, so we cannot just use
    // evaluateIntBinOp
    SimVal *op0 = (SimVal *)evalOperand(currentInst->getOperand(0));
    SimVal *op1 = (SimVal *)evalOperand(currentInst->getOperand(1));

    if (op0->isSym || op1->isSym) {
      frames.back()->insertBinding(currentInst, symCmp(op0, op1));
    } else {
      if (op0->valType == DynValType::INT_VAL &&
          op1->valType == DynValType::INT_VAL) {
        IntVal *tmp0 = (IntVal *)op0;
        IntVal *tmp1 = (IntVal *)op1;
        auto res = cmp(tmp0->intVal, tmp1->intVal);
        frames.back()->insertBinding(currentInst, new IntVal(res));
      } else if (op0->valType == DynValType::POINTER_VAL &&
                 op1->valType == DynValType::POINTER_VAL) {
        auto ptrSize = dataLayout->getPointerSizeInBits();
        PointerVal *tmp0 = (PointerVal *)op0;
        PointerVal *tmp1 = (PointerVal *)op1;
        auto addr0 = tmp0->address;
        auto addr1 = tmp1->address;
        auto res = cmp(APInt(ptrSize, addr0), APInt(ptrSize, addr1));
        frames.back()->insertBinding(currentInst, new IntVal(res));
      } else {
        llvm_unreachable("Illegal icmp compare types");
      }
    }
    break;
  }
  case Instruction::FCmp: {
    auto srcVal0 = evalOperand(currentInst->getOperand(0));
    auto srcVal1 = evalOperand(currentInst->getOperand(1));
    FloatVal *tmp0 = (FloatVal *)srcVal0;
    FloatVal *tmp1 = (FloatVal *)srcVal1;
    auto f0 = tmp0->fpVal;
    auto f1 = tmp1->fpVal;
    auto isF0Nan = std::isnan(f0);
    auto isF1Nan = std::isnan(f1);
    auto bothNotNan = !isF0Nan && !isF1Nan;
    auto eitherIsNan = isF0Nan || isF1Nan;

    switch (cast<CmpInst>(currentInst)->getPredicate()) {
    case CmpInst::FCMP_FALSE:
      frames.back()->insertBinding(currentInst, new IntVal(APInt(1, false)));
    case CmpInst::FCMP_OEQ:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, bothNotNan && f0 == f1)));
    case CmpInst::FCMP_OGT:
      frames.back()->insertBinding(currentInst,
                                   new IntVal(APInt(1, bothNotNan && f0 > f1)));
    case CmpInst::FCMP_OGE:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, bothNotNan && f0 >= f1)));
    case CmpInst::FCMP_OLT:
      frames.back()->insertBinding(currentInst,
                                   new IntVal(APInt(1, bothNotNan && f0 < f1)));
    case CmpInst::FCMP_OLE:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, bothNotNan && f0 <= f1)));
    case CmpInst::FCMP_ONE:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, bothNotNan && f0 != f1)));
    case CmpInst::FCMP_ORD:
      frames.back()->insertBinding(currentInst,
                                   new IntVal(APInt(1, bothNotNan)));
    case CmpInst::FCMP_UEQ:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, eitherIsNan || f0 == f1)));
    case CmpInst::FCMP_UGT:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, eitherIsNan || f0 > f1)));
    case CmpInst::FCMP_UGE:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, eitherIsNan || f0 >= f1)));
    case CmpInst::FCMP_ULT:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, eitherIsNan || f0 < f1)));
    case CmpInst::FCMP_ULE:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, eitherIsNan || f0 <= f1)));
    case CmpInst::FCMP_UNE:
      frames.back()->insertBinding(
          currentInst, new IntVal(APInt(1, eitherIsNan || f0 != f1)));
    case CmpInst::FCMP_UNO:
      frames.back()->insertBinding(currentInst,
                                   new IntVal(APInt(1, eitherIsNan)));
    case CmpInst::FCMP_TRUE:
      frames.back()->insertBinding(currentInst, new IntVal(APInt(1, false)));
    default:
      llvm_unreachable("Illegal fcmp predicate");
    }

    break;
  }

  // Convert instructions ...
  case Instruction::Trunc: {
    auto truncWidth = cast<IntegerType>(currentInst->getType())->getBitWidth();
    IntVal *op0 = (IntVal *)(evalOperand(currentInst->getOperand(0)));
    if (op0->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::EXTRACT, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(new IntVal(APInt(32, 0)));          // offset
      tmp->operands.push_back(new IntVal(APInt(32, truncWidth))); // len
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.trunc(truncWidth)));
    }
    break;
  }
  case Instruction::ZExt: {
    auto extWidth = cast<IntegerType>(currentInst->getType())->getBitWidth();
    IntVal *op0 = (IntVal *)(evalOperand(currentInst->getOperand(0)));
    if (op0->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::ZEXT, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(new IntVal(APInt(32, extWidth)));
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.zext(extWidth)));
    }
    break;
  }
  case Instruction::SExt: {
    auto extWidth = cast<IntegerType>(currentInst->getType())->getBitWidth();
    IntVal *op0 = (IntVal *)(evalOperand(currentInst->getOperand(0)));
    if (op0->isSym) {
      IntVal *tmp = new IntVal(op0->bitWidth, SymExprType::SEXT, "");
      tmp->operands.push_back(op0);
      tmp->operands.push_back(new IntVal(APInt(32, extWidth)));
      frames.back()->insertBinding(currentInst, std::move(tmp));
    } else {
      frames.back()->insertBinding(currentInst,
                                   new IntVal(op0->intVal.sext(extWidth)));
    }
    break;
  }
  case Instruction::FPTrunc: {
    auto srcType = currentInst->getOperand(0)->getType();
    auto dstType = currentInst->getType();
    if (!(srcType->isDoubleTy() && dstType->isFloatTy()))
      llvm_unreachable("Invalid FPTrunc instruction");
    auto srcVal = evalOperand(currentInst->getOperand(0));
    FloatVal *srcFloatVal = (FloatVal *)(srcVal);

    assert(srcFloatVal->isDouble);
    frames.back()->insertBinding(
        currentInst,
        new FloatVal(static_cast<float>(srcFloatVal->fpVal), false));
    break;
  }
  case Instruction::FPExt: {
    auto srcType = currentInst->getOperand(0)->getType();
    auto dstType = currentInst->getType();
    if (!(dstType->isDoubleTy() && srcType->isFloatTy()))
      llvm_unreachable("Invalid FPExt instruction");
    // Extention is a non-op for us
    auto srcVal = evalOperand(currentInst->getOperand(0));
    FloatVal *srcFloatVal = (FloatVal *)(srcVal);
    assert(!srcFloatVal->isDouble);
    frames.back()->insertBinding(currentInst,
                                 new FloatVal(srcFloatVal->fpVal, true));
    break;
  }

  // Since APInt can represent both UI and SI, we process them in the same way
  case Instruction::FPToUI:
  case Instruction::FPToSI: {
    auto intWidth = cast<IntegerType>(currentInst->getType())->getBitWidth();
    auto srcVal = evalOperand(currentInst->getOperand(0));
    auto resVal = new IntVal(
        APIntOps::RoundDoubleToAPInt(((FloatVal *)srcVal)->fpVal, intWidth));
    frames.back()->insertBinding(currentInst, resVal);
    break;
  }
  // Ditto
  case Instruction::UIToFP:
  case Instruction::SIToFP: {
    auto op0 = (IntVal *)(evalOperand(currentInst->getOperand(0)));
    assert(!op0->isSym &&
           "Instruction::U/SIToFP with a symbolic int is not support!");
    auto resVal = new FloatVal(APIntOps::RoundAPIntToDouble(op0->intVal),
                               currentInst->getType()->isDoubleTy());
    frames.back()->insertBinding(currentInst, resVal);
    break;
  }
  case Instruction::IntToPtr: {
    auto op0 = (IntVal *)(evalOperand(currentInst->getOperand(0)));
    assert(!op0->isSym &&
           "Instruction::IntToPtr with a symbolic int is not support!");
    auto ptrSize = dataLayout->getPointerSizeInBits();
    // Look for matching ptrtoint to decide what the address space should be
    auto addrSpace = AddressSpace::GLOBAL;
    auto matchingPtr = findBasePointer(currentInst);
    if (matchingPtr != nullptr && frames.back()->hasBinding(matchingPtr)) {
      auto ptrVal = (PointerVal *)frames.back()->lookup(matchingPtr);
      addrSpace = ptrVal->space;
    }
    auto resVal = new PointerVal(
        op0->intVal.zextOrTrunc(ptrSize).getZExtValue(), addrSpace);
    frames.back()->insertBinding(currentInst, resVal);
    break;
  }
  case Instruction::PtrToInt: {
    auto intWidth = cast<IntegerType>(currentInst->getType())->getBitWidth();
    auto srcVal = evalOperand(currentInst->getOperand(0));
    auto resVal = new IntVal(APInt(intWidth, ((PointerVal *)srcVal)->address));
    frames.back()->insertBinding(currentInst, resVal);
    break;
  }
  case Instruction::BitCast: {
    auto srcType = currentInst->getOperand(0)->getType();
    auto dstType = currentInst->getType();

    if (dstType->isPointerTy()) {
      if (!srcType->isPointerTy())
        llvm_unreachable("Invalid Bitcast");
      auto resVal = evalOperand(currentInst->getOperand(0));
      frames.back()->insertBinding(currentInst, std::move(resVal));
    } else if (dstType->isIntegerTy()) {
      if (srcType->isFloatTy() || srcType->isDoubleTy()) {
        auto srcVal = evalOperand(currentInst->getOperand(0));
        auto resVal =
            new IntVal(APInt::doubleToBits(((FloatVal *)srcVal)->fpVal));
        frames.back()->insertBinding(currentInst, std::move(resVal));
      } else if (srcType->isIntegerTy()) {
        auto resVal = evalOperand(currentInst->getOperand(0));
        frames.back()->insertBinding(currentInst, std::move(resVal));
      } else
        llvm_unreachable("Invalid BitCast");
    } else if (dstType->isFloatTy() || dstType->isDoubleTy()) {
      if (srcType->isIntegerTy()) {
        auto op0 = (IntVal *)(evalOperand(currentInst->getOperand(0)));
        assert(!op0->isSym &&
               "Instruction::BitCast with a symbolic int is not support!");
        auto resVal =
            new FloatVal(op0->intVal.bitsToDouble(), dstType->isDoubleTy());
        frames.back()->insertBinding(currentInst, resVal);
      } else {
        auto resVal = evalOperand(currentInst->getOperand(0));
        frames.back()->insertBinding(
            currentInst,
            new FloatVal(((FloatVal *)resVal)->fpVal, dstType->isDoubleTy()));
      }
    } else
      llvm_unreachable("Invalid Bitcast");

    break;
  }
  case Instruction::Call: {
    ImmutableCallSite cs(currentInst);
    assert(cs);
    auto callTgt = cs.getCalledFunction();
    if (callTgt == nullptr) // if indirect then get function from the map
    {
      auto funPtr = evalOperand(cs.getCalledValue());
      auto funAddr = ((PointerVal *)funPtr)->address;
      callTgt = cast<Function>(addressToFunc.at(funAddr));
    }

    auto argVals = std::vector<DynVal *>();
    for (auto it = cs.arg_begin(), ie = cs.arg_end(); it != ie; ++it) {
      DynVal *a = evalOperand(*it);
      argVals.push_back(a);
    }
    DynVal *retVal;
    if (callTgt->isDeclaration()) {
      // call external function to get result
      retVal = callExternalFunction(cs, callTgt, argVals);
    } else {
      StackFrame *sf = new StackFrame(currentInst, callTgt);
      frames.push_back(sf);
      unsigned i = 0;
      for (auto it = callTgt->arg_begin(), ie = callTgt->arg_end(); it != ie;
           ++it, ++i) {
        frames.back()->insertBinding(it, argVals[i]);
      }
      // change current instruction & call function
      currentInst = &callTgt->getEntryBlock().front();
      retVal = run();
    }
    // add returned valued to frame if necessary
    if (!callTgt->getReturnType()->isVoidTy()) {
      frames.back()->insertBinding(prevInst, retVal);
    }
    break;
  }
  case Instruction::PHI: {
    auto phiNode = dyn_cast<PHINode>(currentInst);
    assert(incomingBB != nullptr && "Incoming BB is a null !");
    int idx = phiNode->getBasicBlockIndex(incomingBB);
    assert(idx != -1 && "PHINode doesn't contain entry for predecessor !");
    auto incomingVal = evalOperand(phiNode->getIncomingValue(idx));
    frames.back()->insertBinding(phiNode, std::move(incomingVal));
    break;
  }

  default: {

    llvm_unreachable("Unsupported instruction");
  }
  }

  // update current instruction
  currentInst = prevInst->getNextNode();
}
