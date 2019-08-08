# clan_check
Check trees for compatibility with defined Clans - "The incontestable clan test" 

__Clan_check has been published as part of the study:__

*Karen Siu-Ting, María Torres-Sánchez, Diego San Mauro, David Wilcockson, Mark Wilkinson, Davide Pisani, Mary J O’Connell, Christopher J Creevey, __Inadvertent Paralog Inclusion Drives Artifactual Topologies and Timetree Estimates in Phylogenomics__, Molecular Biology and Evolution, Volume 36, Issue 6, June 2019, Pages 1344–1356, https://doi.org/10.1093/molbev/msz067*

__Please cite this article if you use Clan_Check in your work.__


## Table of Contents

[Background](https://github.com/ChrisCreevey/clan_check#background)

- [What does it do?](https://github.com/ChrisCreevey/clan_check#what-does-it-do)

- [But... why?](https://github.com/ChrisCreevey/clan_check#but-why)

- [Why test Clans and not Monophyletic Clades?](https://github.com/ChrisCreevey/clan_check#why-test-clans-and-not-monophyletic-clades)

[Installation](https://github.com/ChrisCreevey/clan_check#installation)

[Usage](https://github.com/ChrisCreevey/clan_check#usage)

- [Running Clan_check](https://github.com/ChrisCreevey/clan_check#running-clan_check)

- [Outputs](https://github.com/ChrisCreevey/clan_check#outputs)

- [Interpreting the results](https://github.com/ChrisCreevey/clan_check#interpreting-the-results)

- [Bootstrap information](https://github.com/ChrisCreevey/clan_check#Bootstrap-information)

[Caveats](https://github.com/ChrisCreevey/clan_check#interpreting-the-results)




## Background

### What does it do?

Clan_check analyses single-copy phylogenetic trees to assess if they violate clans* defined by the user. 

>*Clans are defined in the Paper: [Wilkinson, M., McInerney, J.O., Hirt, R.P., Foster, P.G. and Embley, T.M., 2007. Of clades and clans: terms for phylogenetic relationships in unrooted trees. Trends in ecology & evolution, 22(3), pp.114-115.](https://www.cell.com/trends/ecology-evolution/fulltext/S0169-5347(07)00019-5) as:

>"*We propose the term ‘clan’ (from the Gaelic for family) as the unrooted analogue of monophyletic group or clade. There are two complementary clans for every nontrivial split or bipartition in an unrooted tree. Were the tree to be rooted, at least one of the two clans defined by a given split would necessarily be monophyletic. A trivial split in an unrooted tree (i.e. between one of the leaves (terminal taxa, OTUs) and all the others) yields a single clan.*"

>See that paper for a more detailed explanation.

The user provides a file of phylogenetic trees and a file containing the definitions of clans to be tested.

The output is a list for all the trees of each clan using a scoring of 1, 0 or ? where:
  * 1 = the clan is found 
  * 0 = the clan is violated
  * ? = the clan could not be tested becuse too few taxa were present (see below for more details)

A "0" means that two or more of the taxa from that clan were found and they were not in a clan (i.e. they were not together to the exclusion of all other taxa on the tree).

### But... why?

This is designed for large-scale phylogenomic analyses where the user may have thousands of phylogenetic trees. While every effort may have been taken to ensure that the best orthlogs have been chosen, sometimes due to hidden paralogy it is not easy to get the choice right.

In these cases, the only evidence that the gene family may be problematic is when the resulting phylogeentic tree is incorrect for known or "incontestable" groups.

This involves looking for "incontestable relationships" that are not part of the question being asked in the study, but without doubt should exist if the taxa are in the tree.

An example of this is, if a phylogenomic study involved the analysis of the relationships of the birds and used several mammals as an outgroup, then mammals would always be expected to group together.

In this case the mammals are an incontestable clan. If the mammals do not group together, then it is very likely that one of the internal branches of the tree represents a duplication and not a speciation event and so some of the genes in the family may not be orthologs.

`Clan_check` searches for these instances.

If given many such clans to check, researchers can assess the number of these clans that are violated* and decide on the weight of evidence necessary to remove or re-visit the analysis of any gene families.

>* Note: that when we refer to a "violation" of a Clan, this implies that at least 2 taxa from the clan were found in the tree, but they did not group together to the exclusion of everything else as defined by the Clan.

Care must be taken choosing the clans to be tested and in the design of the study to include taxa that allows this test to be made.

You can provide trees and clans of any size and `clan_check` will search for the appropriate sub-set of the clans defined.

For example: 
>if you have a tree with `(A,B,(C,D));` and a clan definition of `C D E`, clan_check will search for clans containing `C` and `D` only. 

If 1 or none of the taxa from a clan are in the tree, clan_check will return a "?" for that test (see output files detail below).

## Why test Clans and not Monophyletic Clades?

Most of the phlyogenetic reconstruction software used today output unrooted trees. This means that you cannot guaruntee that the resulting trees will be rooted correctly. If you can guaruntee that an outgroup is included in your analysis then you could root the resulting trees and test for monophyletic clades. However when dealing with phylogeneomic-scale datasets, this may not be possible (as quite often there will be representatatives of gene famililes missing from some species). Testing for Clans removes this problem.

Furthermore, you may want to define a Clan that may not naturally form a clade in a rooted tree. An example of this would be including the nested earliest diverging species of group which do not form a clade in a rooted tree, but  form a Clan in an unrooted tree.


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

### Running Clan_check

Usage: `clan_check -f [phylip formatted tree file] -c [clan file] `

  Where: [phylip formatted tree file] is a phylip formatted file of trees to be assessed
  
  [clan file] is a file that contains lists of taxa in each line (space seperated) that are to be checked for clans.
  
Two example files are provided:

1) `trees.ph` which contains the trees to be tested:

```
(((a,(b,(c,d))),f),e);
(((a,(b,(e,d))),c),g);
((a,(b,(e,d))),c);
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

### Outputs

The output will be named `[phylip formatted tree file].scores.txt` and will have the following format:


|Tree number | size | Clan 1 | Clan 2 | Clan 3 | Clan 4 | Clan 5 | Clan 6 | boot_average |	boot_max |	boot_min |	boot_stdev |
|------------|------|--------|--------|--------|--------|--------|--------|--------------|----------|----------|------------|
|Tree 1 | 6 | 1 | 1 | 0 | 1 | 0 | ? | 0.000000 |	0.000000 |	0.000000 |	0.000000 |
|Tree 2 | 6 | 0 | 0 | 0 | 0 | 1 | 0 | 0.000000 |	0.000000 |	0.000000 |	0.000000 |
|Tree 3	| 5	| 0	| 0	| 0	| 0	| 1	| ? | 0.000000 |	0.000000 |	0.000000 |	0.000000 |

Where `tree number` is in the same order as the input trees, `size` = the number of taxa in the tree, `Clan x` is the clan definied by the xth line of the clan file, `boot_average` is the average bootstrap support at the internal branches of the tree, `boot_max` is the maximum bootstrap support found in the tree, `boot_min` is the minimum bootstrap support found in the tree and `boot_stdev` is the standard deviation of the bootstrap suports found in the tree.

### Interpreting the results

A "1" in a Clan column  means that this tree did not violate this clan.

A "0" in a Clan column  means that this tree violated this clan.

A "?" in a Clan column  means that there were not enough taxa from the Clan in this tree to carry out the test (minimum required is 2 taxa).

So in the test data:

  * All three trees violated Clan 3 (c d a). i.e. Despite all three trees containing all three taxa from the clan they did not group together to the exclusions of everything else. 

  * Both tree 2 and tree 3 violated clan 1 (c d b). i.e. Despite both tree 2 and 3 containing all three taxa from the clan
they did not group together to the exclusion of everything else.

  * We could not test Clan 6 (g d) against Tree 1 or Tree 3 as neither of those trees had taxon "g" and we need a minimum of 2 taxa from a clan to be in a tree in order to test for violations.

For each tree, you can express the number of Clans violated as a sum, percentage, or treat any violation as a reason to exlucde the tree from further analyses. It all depends on what question you are asking and the level of stringency you wish to apply.

### Bootstrap information

The information on the bootstrap supports is provided to allow users to assess whether the violation of clades by any tree may be related to phylogenetic support.

In the example above there were no boostrap supports on the tree, so all numbers were returned as zero, when this analysis is run using as input the provided example file containing boostraps `trees_wboots.ph`, containing the following information:

```
(((a,(b,(c,d)100)100)100,f)100,e);
(((a,(b,(e,d)20)30)0,c)100,g);
((a,(b,(e,d)2)10)20,c);
```
the following results are returned:

|Tree number | size | Clan 1 | Clan 2 | Clan 3 | Clan 4 | Clan 5 | Clan 6 | boot_average |	boot_max |	boot_min |	boot_stdev |
|------------|------|--------|--------|--------|--------|--------|--------|--------------|----------|----------|------------|
|Tree 1 | 6 | 1 | 1 | 0 | 1 | 0 | ? | 100.000000 |	100.000000 |	100.000000 |	0.000000 |
|Tree 2 | 6 | 0 | 0 | 0 | 0 | 1 | 0 | 37.500000 |	100.000000 |	0.000000 |	37.666298 |
|Tree 3	| 5	| 0	| 0	| 0	| 0	| 1	| ? | 10.666667 |	20.000000 | 2.000000 |	7.363574 |

This may change our interpretation of the previous results as the violation of Clan 3 in Tree 1 may considered more robust as all branches have 100% support, compared to the violation of Clan 3 in Tree 3 where the average bootstrap support was just 10%. 

It is left up to the user to interpret this information in the context of their data and the hypothesis being addressed.

## Caveats

This tool will work with phylogentic trees with branch lengths and treats node labels as phylogenetic supports. However it assumes that there are no other modifications to the standard phylip formatting. It does not work with tree labels (in "[]" after the ";" in the tree).





