# clan_check
Check trees for compatibility with defined monophyletic [edit - not right terminology ] groups - "The incontrovertible clan test" 

## Background
###What does it do?
Clan_check analyses single-copy phylogenetic trees to assess if they violate clans* defined by the user. 

>*see the following paper for a definiton of a "clan"

>Wilkinson, M., McInerney, J.O., Hirt, R.P., Foster, P.G. and Embley, T.M., 2007. Of clades and clans: terms for phylogenetic relationships in unrooted trees. Trends in ecology & evolution, 22(3), pp.114-115.

The user provides a file of phylogenetic trees and a file containing the definitions of clans to be tested.

The output is a list for all the trees of each clan using a scoring of 1 or 0 where 1 = the clan is found and 0 = the clan is violated.

The software will also return a 1 if the none of the taxa from the clan are found in the tree, or if only 1 of the taxa are found.

A "0" means that two or more of the taxa from that clan were found and they were not monophyletic.

### But... why?
This is designed for large-scale phylogenomic analyses where the user may have thousands of phylogenetic trees. While every effort may have been taken to ensure that the best orthlogs have been chosen, sometimes due to hidden paralogy it is not easy to get the choice right.

In these cases, the only evidence that the gene family may be problematic is when the resulting phylogeentic tree is "incorrect".

One way to test for "problematic" gene families is to look for "incontrovertible relationships" that are not part of the question being asked in the study, but without doubt should exist if the taxa are in the tree.

An example of this is, if I was carrying out a phylogenomic study of the fishes and used several mammals as an outgroup, then I should never expect the mammal clan to be paraphyletic [edit - whats the equivalent of paraphyly for a clan?].

In this case the mammals are an incontrovertible clan. If the mammals are paraphyletic with the fishes, then it is very likely that one of the internal branches of the tree represents a duplication and not a speciation event, and so they are not all orthologs.

Clan_check searches for these instances.

If given many such clans to check, researchers can assess the number of these clans that are violated and decide on the weight of evidence necessary to remove or re-visit the analysis of that gene family.

Care must be taken choosing the clans to be tested and in the designing of the study, to include taxa that allows this test to be made.

You can provide trees and clans of any size and `clan_check` will search for the appropriate sub-set of the clans defined.

For example: 
>if you have a tree with `(A,B,(C,D));` and a clan definition of `C D E`, clan_check will search for monophylies of `C` and `D` only. 

If only 1 of the taxa from a clan are in the tree, clan_check will assume that the clan is not violated, and return a "1" for that test (see output files detail below).

## Installation

To install the software download the file "clan_check.c" (or if you have git installed use the command:

```
git clone https://github.com/ChrisCreevey/clan_check.git
```

Then (on a unix-based operationing system) type the command:

```
cc clan_check.c -o clan_check -lm
```

It is advisable to copy "clan_check" to somewhere on your path (like `~/bin` ) in order to make sure that it is availalbe everywhere on your system.


## Usage

Usage: `clan_check -f [phylip formatted tree file] -c [clan file] `

  Where: [phylip formatted tree file] is a phylip formatted file of trees to be assessed
  
  [clan file] is a file lists of taxa in each line (space seperated) that are to be checked for monophylies.
  
Two example files are provided:

1) `trees.ph` which contains the trees to be tested:

```
(((a,(b,(c,d))),f),e);
(((a,(b,(e,d))),c),g);
```
These trees can be rooted or unrooted. `clan_check` will unroot all rooted trees before carrying out the analysis.


2) `clans.txt` which contains the clans to be tested (one clan per line, with the taxa sperated by spaces)

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
Tree number size    Clan 1 Clan 2 Clan 3 Clan 4 Clan 5 Clan 6
Tree 1      6       1       1       0       1       1       1
Tree 2      6       0       1       0       1       1       1
```
Where `tree number` is in the same order as the input trees, `size` = the number of taxa in the tree, `Clan x` is the clan definied by the xth line of the clan file.

In this example Clan 3 defined as having the monophyly of "c d a" was violated in both tree 1 and tree 2.

In this result Tree 2 violated 2 of the clans and tree 1 violoated 1.

## Caveats

This tool will work with phylogenies with branch lengths and node labels (like bootstrap supports) and ignores both. However it assumes that there are no other modifications to the standard phylip formatting. It does not work with tree labels (in "[]" after the ";" in the tree).





