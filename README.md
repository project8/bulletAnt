bulletAnt
=========

bulletAnt is a graphical user interface for the hand-scanning of waterfall plots in Project 8 using ROOT

Dependencies
------------
 - ROOT (https://root.cern.ch/) (6.x should work)

Instructions for Use
--------------------

  As ROOT is an interpreted language, there is no installation required. bulletAnt can be run by typing:
 
  ```
  $  root
  ```
 To first start an interactive seesion of ROOT.
  ```
  root [0]  .L HandScanAssistant.C
  root [1]  HandScanAssistant("/path/to/spectrograms_rid00000XXXX_000000YYY_to_000000ZZZ.root")
  ```
 Where the argument is the path to the root file you downloaded from DIRAC, on your local file system, in quotations.

 A brief tutorial is placed is in the Tutorial directory, outlining the basic usage.
