bulletAnt
=========

bulletAnt is a graphical user interface for the hand-scanning of waterfall plots in Project 8 using ROOT

Dependencies
------------
 - ROOT (https://root.cern.ch/) (6.x should work)
 - libyaml


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

 A new window containing the GUI will then pop up on your screen. The bulletAnt window has two button panels, on the bottom and on the left,
 as well as spectrogram convas on the right hand side. 

 To add a track, click on the "Create Track" button, and manually drag the resulting white line on the plot, to on top of the track in question. Check boxes denote whether you think
 the track is either a sideband, or part of a curved structure. Note once you go onto another track, you can continue to move previous tracks, however you cannot (currently) change
 whether they are sidebands or not.

 When you have marked all of the tracks/ features in all of the spectrograms, the "Write YAML" button will produce a YAML file, which can then be uploaded to DIRAC,
 to verify the performance of our reconstruction algorithms. It is very important that you write before pressing "Exit", as doing so will result in losing your work!
