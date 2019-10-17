/*
 * Z3Solver.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "Z3Solver.h"

Z3Solver::Z3Solver() : s(solver(c)) {}

Z3Solver::~Z3Solver() {}

expr Z3Solver::generateZ3Expr(shared_ptr<DynVal> ex, context &c) {
  if (ex->valType == DynValType::INT_VAL) {
    shared_ptr<IntVal> si = std::static_pointer_cast<IntVal>(ex);
    switch (si->symExprType) {
    case SymExprType::VAR: {
      return c.bv_const(si->name.c_str(), si->bitWidth);
    }
    case SymExprType::EQ: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l == r);
    }
    case SymExprType::NE: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l != r);
    }
    case SymExprType::ULT:
    case SymExprType::SLT: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l < r);
    }
    case SymExprType::ULE:
    case SymExprType::SLE: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l <= r);
    }
    case SymExprType::UGT:
    case SymExprType::SGT: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l > r);
    }
    case SymExprType::UGE:
    case SymExprType::SGE: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l >= r);
    }
    case SymExprType::ADD: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l + r);
    }
    case SymExprType::SUB: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l - r);
    }
    case SymExprType::MUL: {
      expr l = generateZ3Expr(si->operands.at(0), c);
      expr r = generateZ3Expr(si->operands.at(1), c);
      return (l * r);
    }
      // TODO: Support more expression types

    default: {
      errs() << "Unsupported Expression Conversion!\n";
      si->print();
      errs() << "\n";
    }
    }
    return c.bool_val(false);
  } else if ((ex->valType == DynValType::INT_VAL)) {
    APInt intVal = static_pointer_cast<IntVal>(ex)->intVal;
    return c.bv_val(*intVal.getRawData(), intVal.getBitWidth());
  } else {
    errs() << "Unsupported DynValue Conversion!\n";
    ex->print();
    errs() << "\n";
    return c.bool_val(false);
  }
}

Z3SolverResult Z3Solver::check(std::map<shared_ptr<IntVal>, bool> &pcs,
                               shared_ptr<IntVal> &q, bool isTrue) {
  //  for (std::map<shared_ptr<SymIntValue>, bool>::iterator it = pcs.begin(),
  //                                                         ie = pcs.end();
  //       it != ie; ++it) {
  //    it->first->print();
  //    errs() << "->" << it->second << "\n";
  //  }
  //  errs() << "--------------\n";
  //  q->print();
  //  errs() << "=>" << isTrue << "\n";
  //  errs() << "==============\n";

  ++Statistics::solverCheckingCounter;
  auto start = std::chrono::high_resolution_clock::now();

  s.push();
  Z3SolverResult res = Z3SolverResult::UNKNOWN;
  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
                                                    ie = pcs.end();
       it != ie; ++it) {
    expr pc = generateZ3Expr(it->first, c);
    if (it->second) {
      s.add(pc);
    } else {
      s.add(not(pc));
    }
  }

  expr pc = generateZ3Expr(q, c);
  if (isTrue) {
    s.add(pc);
  } else {
    s.add(not(pc));
  }
  check_result cr = s.check();
  if (cr == sat) {
    res = Z3SolverResult::SAT;
  } else if (cr == unsat) {
    res = Z3SolverResult::UNSAT;
  }
  s.pop();

  auto finish = std::chrono::high_resolution_clock::now();
  Statistics::solverCheckingTime += (finish - start);

  return res;
}

void Z3Solver::printModel(std::map<shared_ptr<IntVal>, bool> &pcs) {
  s.push();
  // add path constraints
  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
                                                    ie = pcs.end();
       it != ie; ++it) {
    expr pc = generateZ3Expr(it->first, c);
    if (it->second) {
      s.add(pc);
    } else {
      s.add(not(pc));
    }
  }
  check_result res = s.check();
  if (res == sat) {
    model m = s.get_model();
    for (unsigned i = 0; i < m.size(); i++) {
      func_decl v = m[i];
      assert(v.arity() == 0);
      errs() << v.name().str() << " = " << m.get_const_interp(v).to_string()
             << "\n";
    }
  }
  s.pop();
}
