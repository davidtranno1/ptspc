### Probabilistic Travelling Salesman Problem with Crowdsourcing

Code companion for the paper ``Exact, Heuristic and Machine Learning Approaches to The Probabilistic Travelling Salesman Problem with Crowdsourcing'' by Alberto Santini, Ana Viana, Xenia Klimentova, João Pedro Pedroso.

The source code is in directory `src` and can be built using the accompanying `CMakeList.txt`.
The instances are in directory `instances`.

For licensing information, refer to file `LICENSE`.

#### How to run the programme

Calling the executable with `--help` provides the necessary instructions:

```
Usage:
  stochastic_tsp [options]
Available options:
  -i, --instance          PTSPC instance file
  -s, --solver            One of: enum, fwd, bck, fwdbck, bckfwd (default is e-
                            num)
                            * enum: enumerates all possible offered sets
                            * fwd: forward stepwise heuristic
                            * bck: backward stepwise heuristic
                            * fwdbck: alternates one forward and one backward 
                            step
                            * bckfwd: alternates one backward and one forward 
                            step
                            * tsp: just solves the tsp over all delivery points
  -e, --exp-cost-solver   One of: exact, mc, rtlr, rt (default is exact)
                            * exact: computes the exact expected cost
                            * mc: approximates the expected cost via monte-car-
                            lo simulation
                            * rtlr: approximates the expected cost via a regre-
                            ssion tree with linear regression in its leaves
                            * rt: approximates the expected cost via a regress-
                            ion tree
  -t, --tsp-solver        One of: discorde, cplexbc (deafult is discorde)
                            * discorde: uses the concorde TSP solver via the d-
                            iscode api
                            * cplexbc: solves the TSP using a simple branch-an-
                            d-cut algorithm and the solver CPLEX
  -m, --monte-carlo-n     Number of Monte Carlo simulations (default is 20)
  -T, --ml-training-time  Number of seconds to spend to gather traning data (d-
                            efault is 60
  -r, --recompute-exact   Whether to compute the exact expected cost of the be-
                            st set found via heuristic expected cost solvers
  -d, --draw-solution     Whether to draw a picture of the solution
  -c, --create-dataset    Whether to create the dataset while solving the prob-
                            lem
  -R, --randomise-sets    Whether to randomise the starting sets for fwd, bck, 
                            fwdbck and bckfwd heuristics.
  -h, --help              Displays this help screen
```