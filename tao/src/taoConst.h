
////////////////////////////////////////////////////////////////////////
/*
        This file is a part of an interpreter for Tao, a high level
        object-oriented computing and scripting language.

        Copyright (C) 2004-2005, Fu Limin.
        Contact: fu.limin.tao@gmail.com, limin.fu@ircc.it

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
////////////////////////////////////////////////////////////////////////////
#ifndef TAO_CONST_H
#define TAO_CONST_H

//#include"taoObject.h"
//#include"taoNumeric.h"
//#include"taoRecycler.h"

enum TaoRTTI {
  TAO_Base = 0,
  TAO_Reference = 1,
  TAO_ConstRefer = 2,
  TAO_DataRefer = 3,
  TAO_ParamRefer = 4,
  TAO_ShareRefer = 5,

  TAO_ArithBase = 10,
  TAO_ArithVariable = 11,
  TAO_ArithChain = 12,
  TAO_ArithConst = 13,
  TAO_ArithNode = 14,
  TAO_ArithTree = 15,
  TAO_ArithArray = 16,
  TAO_FunCall = 17,
  TAO_Chain = 18,
  TAO_RgxMatch = 19,

  TAO_Number = 21,
  TAO_String = 22,
  TAO_Object = 23,
  TAO_Fin = 24,
  TAO_Fout = 25,

  TAO_Array = 31,
  TAO_Hash = 32,
  TAO_Complex = 33,
  TAO_NumArray = 34,
  TAO_RealArray = 35,
  TAO_CompArray = 36,

  TAO_Module = 51,
  TAO_Routine = 52,
  TAO_Class = 53,

  TAO_CppObject = 100,
  TAO_Plugin = 102,
  TAO_AddOn = 103,

  TAO_NameSpace = 111,
  TAO_Library = 112
};
enum TaoArithOperType {

  TAO_REGEX_EQ = 0,
  TAO_REGEX_NE = 1,
  TAO_REGEX_ALL = 2,

  TAO_OPER_IF = 3,
  TAO_COLON = 4,

  TAO_ASSIGN = 5,
  TAO_ASSIGN_PLUS = 6,
  TAO_ASSIGN_MINUS = 7,
  TAO_ASSIGN_TIMES = 8,
  TAO_ASSIGN_DIVIDE = 9,
  TAO_ASSIGN_MOD = 10,
  TAO_ASSIGN_POW = 11,
  TAO_ASSIGN_AND = 12,
  TAO_ASSIGN_OR = 13,

  TAO_AND = 14,
  TAO_OR = 15,

  TAO_SMALLER = 16,
  TAO_LARGER = 17,
  TAO_EQUAL = 18,
  TAO_NOT_EQUAL = 19,
  TAO_NOT_LARGER = 20,
  TAO_NOT_SMALLER = 21,

  TAO_PLUS = 22,
  TAO_MINUS = 23,
  TAO_TIMES = 24,
  TAO_DIVIDE = 25,
  TAO_PAIR_TIMES = 26,
  TAO_PAIR_DIVIDE = 27,
  TAO_MOD = 28,
  TAO_POW = 29,
  TAO_TYPE_SAME = 30,
  TAO_TYPE_DIFF = 31,

  TAO_NOT = 40,
  TAO_PPLUS = 41,
  TAO_MMINUS = 42,
  TAO_TRANSPOSE = 43,
  TAO_INVERSE = 44,
  TAO_IMAGINARY = 45,

};
enum TaoInternFuncTypes {
  //=================================
  // Built in functions:
  //=================================
  //	Basic mathematical function:
  TAO_abs = 1,
  TAO_acos = 2,
  TAO_asin = 3,
  TAO_atan = 4,
  TAO_cos = 5,
  TAO_cosh = 6,
  TAO_exp = 7,
  TAO_log = 8,
  TAO_sin = 9,
  TAO_sinh = 10,
  TAO_sqrt = 11,
  TAO_tan = 12,
  TAO_tanh = 13,

  TAO_arg = 14,
  TAO_norm = 15,

  TAO_rand = 16,
  TAO_randgs = 17,
  TAO_srand = 18,

  // Basic numeric array functions:
  // Including "magic" function apply() and noapply():
  // Type I: run by TaoNumArray::basicMathFunct()
  TAO_max = 20,
  TAO_min = 21,
  TAO_mean = 22,
  TAO_stdev = 23,
  TAO_varn = 24,
  TAO_sum = 25,
  TAO_prod = 26,
  // Type II:
  TAO_count = 27,
  TAO_permute = 28,
  TAO_invpermute = 29,
  TAO_convolute = 30,

  //	"Magic" functions:
  TAO_apply = 40,
  TAO_noapply = 41,
  TAO_iterate = 42,
  TAO_iterget = 43,
  TAO_repeat = 44,
  TAO_sort = 45,
  TAO_which = 46,
  TAO_numarray = 47,

  // Others:
  TAO_about = 60,
  TAO_compile = 61,
  TAO_eval = 62,
  TAO_import = 63,
  TAO_load = 64,
  TAO_system = 65,
  TAO_type = 66,
  TAO_number = 67,
  TAO_pack = 68,
  TAO_unpack = 69,
  TAO_time = 70,
  TAO_astime = 71,
  TAO_asctime = 72,

  //=================================
  // Built in methods:
  //=================================
  TAO_erase = 80,
  TAO_flat = 81,
  TAO_insert = 82,
  TAO_keys = 83,
  TAO_reshape = 84,
  TAO_resize = 85,
  TAO_size = 86,
  TAO_values = 87,

  // File IO:
  TAO_open = 101,
  TAO_close = 102,
  TAO_print = 103,
  TAO_read = 104
};

#endif
