Tutorial
--------------------

bulletAnt is a graphical user interface aimed at assisting users in handscanning of waterfall plots in Project 8.

  ```
  $  root
  ```
 To first start an interactive seesion of ROOT.
  ```
  root [0]  .L /path/to/bulletAnt/HandScanAssistant.C
  root [1]  HandScanAssistant("/path/to/spectrograms_rid00000XXXX_000000YYY_to_000000ZZZ.root")
  ```
 Where the argument in the second command is the path to the root file you downloaded from DIRAC, on your local file system, in quotations.

 The following terminal command also works, though it is a bit inconvenient:
  ```
  $ root -l '/path/to/bulletAnt/HandScanAssistant.cc("/path/to/spectrograms_rid00000XXXX_000000YYY_to_000000ZZZ.root")'
  ```

 Once opened, a new window containing the GUI will pop up on your screen, with two button panels, on the bottom and on the left, as well as the spectrogram on the right. 
 Below is an image of the GUI.

![bulletAnt GUI](https://github.com/project8/bulletAnt/blob/master/Tutorial/bulletAntInterface.png?raw=true "bulletAnt Graphical User Interface")

**Create Track:**
Creates a white line segment in the center of your window. Drag and drop the position of the line with the underlying electron track. 
If the track is curved or a sideband, click one or both of the the checkboxes underneath this button. Note that these buttons apply only to
the most recent track, and cannot access older tracks. 

**Create Curve:**
 Creates a red line segment, that should be specifically placed around curves, which you believe are part of the carrier (and are not sidebands). Try to estimate
the start and end of the curving if it is difficult.

**Other Feature:**
 Will produce a white dot that can be moved to signify if there is anything unusual in the spectrogram. A popup dialog will ask you for a brief comment on the feature,
which will be logged in the output file. If you need to delete an object (track, curve, or other feature), move it entirely to the left of the spectrogram (where time is negative). 
If it a line object, make sure to move both ends to t < 0.

**Write To YAML:**
 This button writes the tracks, curves, and other features to a YAML file in the same directory as your .root data files. A popup dialog will ask you for your name, in an attempt to
autogenerate the correct filename for DIRAC, as well as the metadata. Once you have written, plase fill out the metadata "spectrogram_file" entry with the path on DIRAC. 

**Load From YAML:**
 This button lets you load in the tracks, curves, and other features from a YAML file, into the bulletAnt environment. This offers a convenient way to save your progress: just press the
"Write to YAML" button, and pick up later by pressing the "Load From YAML" button, which launches a file dialog of your directory system.

**Refresh Plot:**
 Redraws plot, zooms in to lower left hand corner. May be removed in future versions.

**Sliders:**
 Pair of double sliders, for the X and Y axes, respectively. Grabbing the center of the slider button and dragging will slide your focus along the XY axes. If you grab the
 edge of the slider button and drag, you can zoom in or out. By default, the spectrogram is zoomed in as a) operations are quicker on zoomed-in plots, as there is less data, and
 b) when you create tracks, try to really zoom in to get the best accuracy you can

**Previous/ Next Spectrogram:**
 Navigation bars. Located in the lower panel.

**Exit:**
 Exits the application. Note that if you exit without writing, your work is gone forever, so don't do it.

