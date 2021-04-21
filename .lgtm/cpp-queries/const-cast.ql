/**
 * @name const_cast<>() removing constness
 * @description Using const_cast<>() to remove constness is extremely discouraged
 * @kind problem
 * @problem.severity warning
 * @precision medium
 * @id cpp/const-cast
 * @tags reliability
 *       correctness
 *       security
 *       external/cppcoreguidelines/pro-type-constcast
 */

import cpp

from ConstCast cast_
where
	not cast_.isCompilerGenerated() and
	not (cast_.getUnderlyingType().isConst() or cast_.getUnderlyingType().isDeeplyConstBelow())
select cast_, "Casting away constness - const_cast<>()"
