/*
 * Z3Solver.cpp
 *
 *  Created on: 17 Oct 2019
 *      Author: xuanlinhha
 */

#include "Z3Solver.h"

Z3Solver::Z3Solver() : s(solver(c)) {}

Z3Solver::~Z3Solver() {}

expr Z3Solver::generateZ3Expr(shared_ptr<SimVal> ex, context &c) {
  if (ex->isSym) {
    switch (ex->symExprType) {
    case SymExprType::VAR: {
      return c.bv_const(ex->name.c_str(), ex->getBitWidth());
    }
    case SymExprType::EQ: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l == r);
    }
    case SymExprType::NE: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l != r);
    }
    case SymExprType::ULT:
    case SymExprType::SLT: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l < r);
    }
    case SymExprType::ULE:
    case SymExprType::SLE: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l <= r);
    }
    case SymExprType::UGT:
    case SymExprType::SGT: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l > r);
    }
    case SymExprType::UGE:
    case SymExprType::SGE: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l >= r);
    }
    case SymExprType::ADD: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l + r);
    }
    case SymExprType::SUB: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l - r);
    }
    case SymExprType::MUL: {
      expr l = generateZ3Expr(ex->operands.at(0), c);
      expr r = generateZ3Expr(ex->operands.at(1), c);
      return (l * r);
    }
      // TODO: Support more expression types

    default: {
      errs() << "Unsupported Expression Conversion!\n";
      ex->print(&errs());
      errs() << "\n";
    }
    }
    return c.bool_val(false);
  } else {
    APInt intVal = static_pointer_cast<IntVal>(ex)->intVal;
    return c.bv_val(*intVal.getRawData(), intVal.getBitWidth());
  }
}

SolverResult Z3Solver::check(std::map<shared_ptr<IntVal>, bool> &pcs,
                             shared_ptr<IntVal> &q, bool qval) {

  ++Statistics::solverCheckCounter;
  auto start = std::chrono::high_resolution_clock::now();

  s.push();
  SolverResult res = SolverResult::UNKNOWN;
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
  if (qval) {
    s.add(pc);
  } else {
    s.add(not(pc));
  }
  check_result cr = s.check();
  if (cr == sat) {
    res = SolverResult::SAT;
  } else if (cr == unsat) {
    res = SolverResult::UNSAT;
  }
  s.pop();

  auto finish = std::chrono::high_resolution_clock::now();
  Statistics::solverTime += (finish - start);

  // for debug
  //  for (std::map<shared_ptr<IntVal>, bool>::iterator it = pcs.begin(),
  //                                                    ie = pcs.end();
  //       it != ie; ++it) {
  //    it->first->print(&errs());
  //    errs() << "->" << it->second << "\n";
  //  }
  //  errs() << "--------------\n";
  //  q->print(&errs());
  //  errs() << "=>" << qval << "\n";
  //  errs() << "Z3Solver::check pcs size = " << pcs.size() << "\n";
  //  errs() << "Z3Solver::check result = " << getStrResult(res) << "\n";
  //  errs() << "==============\n";

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

string Z3Solver::getStrResult(SolverResult sr) {
  if (sr == SolverResult::SAT) {
    return "SAT";
  } else if (sr == SolverResult::UNSAT) {
    return "UNSAT";
  } else {
    return "UNKNOWN";
  }
}
