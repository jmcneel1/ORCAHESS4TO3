# ORCAHESS4TO3

This Small Utility Will Create A Hessian With Formatting of ORCA version 3
rather than 4. In particular, this was developed to allow for processing with
qmdff from the Grimme group.

The differences are as follows:

1. In version 4, ORCA outputs 5 columns for outputting the normal modes and
   the hessian. In version 3, ORCA uses 6 columns.
2. In version 4, ORCA uses scientific notation for the data, but version 3
   uses fixed notation.
3. ORCA 4 uses %.10E, 3 uses %.6f
4. Column width for v4 is setw(19), for v3 is setw(11)
5. Atoms, v4 uses .5f for weights, v4 uses .4f
6. Atoms, v4 uses .12f for coords, v3 uses .6f [setw(19),setw(13)]
7. Dipole derivatives, setw(20) .10E vs. setw(13) .6f
