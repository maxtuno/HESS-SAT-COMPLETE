# HESS SAT COMPLETE

### Copyright (c) 2012-2022 Oscar Riveros (@maxtuno, Chile)

The original HESS (Hyper Exponential Space Sorting) is a polynomial black-box optimization algorithm, that work very well with any NP-Complete, or NP-Hard problem, at 2021 thanks to suggestions of Daniel Mattes, work like a complete algorithm.

the follow questions remains open:

1) Is original HESS, with a MaxSAT Oracle, candidate to proof that P=NP, because almost in practice, alwais break the theoretical inaproximability ratio. (this separates the P=NP from P!=NP)
2) Exist other oracle, like TSP, where the theoretical inapproximability ratio is broken?
3) Is possible extend HESS with heuristics, like DPLL algorithm or CDCL?
4) In definitive, is possible that HESS help to solve the P vs NP question?
5) Proof or disproof that HESS SAT is Complete for SAT.
6) Complexity?

Thanks to all my supporters across the ages, with special mention of all people of http://www.foresta.io, ofcourse Daniel Mattes, and my loved wife.

usage: hess-sat cnf-instance

Remarks: hess_sat_boost.cc with an alternative free collision hashing with boost.

This software is released with MIT licence:


Copyright (c) 2012-2022 Oscar Riveros (@maxtuno, Chile).

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
