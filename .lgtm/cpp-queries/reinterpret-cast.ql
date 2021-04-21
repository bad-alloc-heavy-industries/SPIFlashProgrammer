/**
 * @name Potential UB - reinterpret_cast<>()
 * @description Using reinterpret_cast<>() is, except in rare circumstance, UB-invoking
 *    Please do not use reinterpret_cast<>()
 * @kind problem
 * @problem.severity warning
 * @precision medium
 * @id cpp/reinterpret-cast
 * @tags reliability
 *       correctness
 *       security
 *       external/cppcoreguidelines/pro-type-reinterpretcast
 */

import cpp

from ReinterpretCast cast_
where
  not cast_.isCompilerGenerated()
select cast_, "Potential UB - reinterpret_cast<>()"
