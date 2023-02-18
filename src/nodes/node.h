#pragma once
#include "nodes/base.h"
#include "nodes/variable/variable.h"
#include "nodes/variable/array.h"
#include "nodes/variable/enum.h"
#include "nodes/variable/pointer.h"
#include "nodes/variable/composite.h"
#include "nodes/cast.h"
#include "nodes/io/io.h"
#include "nodes/io/file.h"

#include "nodes/eval/arithmetic.h"
#include "nodes/eval/stringcat.h"
#include "nodes/eval/comparison.h"
#include "nodes/eval/logic.h"

#include "nodes/selection/ifStatement.h"
#include "nodes/selection/case.h"

#include "nodes/loop/while.h"
#include "nodes/loop/repeatUntil.h"
#include "nodes/loop/for.h"
#include "nodes/loop/control.h"

#include "nodes/functions/procedure.h"
#include "nodes/functions/function.h"
