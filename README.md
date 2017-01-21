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

## Installation

To install the software download the file "clade_check.c" (or if you have git installed use the command:

```

