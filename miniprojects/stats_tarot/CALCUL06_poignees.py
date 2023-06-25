#!/usr/bin/env python
# coding: utf-8

from utils import C, A, f, assert_almost_equal, display_percent


exactement_15_atouts = A(15,22) / A(15,78)
exactement_14_atouts = A(14,22) * C(1,56) * 15 / A(15,78)
exactement_13_atouts = A(13,22) * C(2,56) * 15*14 / A(15,78)
exactement_12_atouts = A(12,22) * C(3,56) * 15*14*13 / A(15,78)
exactement_11_atouts = A(11,22) * C(4,56) * 15*14*13*12 / A(15,78)
exactement_10_atouts = A(10,22) * C(5,56) * 15*14*13*12*11 / A(15,78)
exactement_9_atouts = A(9,22)   * C(6,56) * 15*14*13*12*11*10 / A(15,78)
exactement_8_atouts = A(8,22)   * C(7,56) * 15*14*13*12*11*10*9 / A(15,78)

exactement_7_atouts = A(7,22)    * C(8,56) * 15*14*13*12*11*10*9*8 / A(15,78)
exactement_6_atouts = A(6,22)    * C(9,56) * 15*14*13*12*11*10*9*8*7 / A(15,78)
exactement_5_atouts = A(5,22)   * C(10,56) * 15*14*13*12*11*10*9*8*7*6 / A(15,78)
exactement_4_atouts = A(4,22)   * C(11,56) * 15*14*13*12*11*10*9*8*7*6*5 / A(15,78)
exactement_3_atouts = A(3,22)   * C(12,56) * 15*14*13*12*11*10*9*8*7*6*5*4 / A(15,78)
exactement_2_atouts = A(2,22)   * C(13,56) * 15*14*13*12*11*10*9*8*7*6*5*4*3 / A(15,78)
exactement_1_atouts = A(1,22)   * C(14,56) * 15*14*13*12*11*10*9*8*7*6*5*4*3*2 / A(15,78)
exactement_0_atouts = A(15,56)  / A(15,78)

exactement_triple_poignee = exactement_15_atouts + exactement_14_atouts + exactement_13_atouts
exactement_double_poignee = exactement_triple_poignee + exactement_12_atouts + exactement_11_atouts + exactement_10_atouts
exactement_simple_poignee = exactement_double_poignee + exactement_9_atouts + exactement_8_atouts

au_moins_poignee_simple = exactement_triple_poignee + exactement_double_poignee + exactement_simple_poignee
au_moins_poignee_double = exactement_triple_poignee + exactement_double_poignee
aucune_poignee = 1.0 - au_moins_poignee_simple

assert_almost_equal(aucune_poignee, (
    exactement_7_atouts +
    exactement_6_atouts +
    exactement_5_atouts +
    exactement_4_atouts +
    exactement_3_atouts +
    exactement_2_atouts +
    exactement_1_atouts +
    exactement_0_atouts
    )
)



print ""
print "Probabilité d'avoir dans ma main :"
print "   - aucune poignee         = ", display_percent(aucune_poignee)
print "   - poignee simple ou plus = ", display_percent(au_moins_poignee_simple)
print "   - poignee double ou plus = ", display_percent(au_moins_poignee_double)
print "   - poignee triple         = ", display_percent(exactement_triple_poignee)

