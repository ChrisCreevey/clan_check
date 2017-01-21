# clade_check
Check trees for compatibility with defined monophyletic groups - "The incontrovertible clade test" 

## Background
###What does it do?
Clade_check analyses single-copy phylogenetic trees to assess if they violate clades defined by the user (Strictly speaking they are clans as the trees are unrooted).

The user provides a file of phylogenetic trees and a file containing the definitions of clades to be tested.

The output is a list for all the trees of each clade using a scoring of 1 or 0 where 1 = the clade is found and 0 = the calde is violated.

The software will also return a 1 if the none of the taxa from the clade are found in the tree, or if only 1 of the taxa are found.

A "0" means that two or more of the taxa from that clade were found and they were not monophyletic.

### But... why?
This is designed for large-scale phylogenomic analyses where the user may have thousands of phylogenetic trees. While every effort may have been taken to ensure that the best orthlogs have been chosen, sometimes due to hidden paralogy it is not easy to get the choise right.

In these cases, the only evidence that the gene family may be problematic is when the resulting phylogeentic tree is incorrect.

One way to test for "problematic" gene families is to look for "incontrovertible relationships" that are not part of the question being asked in the study, but without doubt should exist if the taxa are in the tree.

An example of this is, if I was carrying out a phylogenomic study of the fishes and used several mammals as an outgroup, then I should never expect the mammal clade to be paraphyletic.

In this case the mammals are an incontrovertible clade. If the mammals are paraphyletic with the fishes, then it is very likely that one of the internal branches of the tree represents a duplication and not a speciation event, and so they are not all orthologs.

Clade_check searches for these instances.

If given many such clades to check, researchers can assess the number of these clades that are violated and decide on the weight of evidence necessary to remove of re-visit the analysis of that gene family.

Care must be taken choosing the clades to be tested and in the designing of the study, to include taxa that allows this test to be made.

## Installation

To install the software download the file "clade_check.c" (or if you have git installed use the command:

```
git clone https://github.com/ChrisCreevey/clade_check.git
```

Then (on a unix-based operationing system) type the command:

```
cc clade_check.c -o clade_check -lm
```

It is advisable to copy "clade_check" to somewhere on your path (like `~/bin` ) in order to make sure that it is availalbe everywhere on your system.


## Usage

Usage: `clade_check -f [phylip formatted tree file] -c [clade file] `

  Where: [phylip formatted tree file] is a phylip formatted file of trees to be assessed
  
  [clade file] is a file lists of taxa in each line (space seperated) that are to be checked for monophylies.
  
Two example files are provided:

1) `trees.ph` which contains the trees to be tested:

```
(((a,(b,(c,d))),f),e);
(((a,(b,(e,d))),c),g);
```

2) `clades.txt` which contains the clades to be tested (one clade per line, with the taxa sperated by spaces)

```
c d b
a f e
c d a
c d
c g a
g d
```

The output will be named `[phylip formatted tree file].scores.txt` and will have the following format:

```
Tree number size    Clade 1 Clade 2 Clade 3 Clade 4 Clade 5 Clade 6
Tree 1      6       1       1       0       1       1       1
Tree 2      6       0       1       0       1       1       1
```
Where `tree number` is in the same order as the input trees, `size` = the number of taxa in the tree, `Clade x` is the clade definied by the xth line of the clade file.

In this example Clades 3 defined as having the monophyly of "c f e" was violated in both tree 1 and tree 2.

In this result Tree 2 violated 2 of the clades and tree 1 violoated 1.

## Caveats

This tool will work with phylogenies with branch lengths and node labels (like bootstrap supports) and ignores both. However it assumes that there are no other modifications to the standard phylip formatting. It does not work with tree labels (in "[]" after the ";" in the tree).





