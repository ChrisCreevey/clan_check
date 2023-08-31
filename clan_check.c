#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TEMP
#define TEMP 2
#endif


void unroottree(char * tree);
void assess_bootstraps(char * tree, float * average, float * max, float * min, float * stdev );

int main (int argc, char *argv[])
  {
  char *treefilename=NULL, *cladefilename=NULL, outfilename[10000], c, **clades=NULL, **trees=NULL, *tree=NULL, *constraint=NULL, taxa[1000][1000], clanlist[1000][1000], *token=NULL, string[1000], *testtree=NULL, treename[1000], tmpconstraint[10000];
  FILE *treefile=NULL, *cladefile=NULL, *outfile=NULL;
  int fflag=0, cflag=0, i=0, j=0, k=0, g=0, l=0, m=0, n=0, p=0, numfails=0, numclades=0, numtrees=0, linelength=0, treelength=0, numtaxa=0, numintree=0, numinconstraint=0, clann_size=0, foundclade = FALSE, foundtaxa=FALSE;
  float boot_average, boot_max, boot_min, boot_stdev;

  if(argc < 2)
  	{
    printf("\n\nclan_check: Assess phylogenies for compatibility with defined clans\n\n Usage: \"clan_check -f <phylip formatted tree file> -c <clan file> \"\n\n\tWhere: <phylip formatted tree file> is a phylip formatted file of trees to be assessed\n\t<clan file> is a file lists of taxa in each line (space seperated) that belong to the clan to be tested\n\n");
  	}
  while ((c = getopt(argc, argv, "f:c:h")) != -1)
    {   
      switch (c) 
      {
      case 'c':
        cflag = 1;
        cladefilename = optarg;
        break;
      case 'f':
        fflag = 1;
        treefilename = optarg;
        break;
      case 'h':
	    printf("\n\nclan_check: Assess phylogenies for compatibility with defined monophylies\n\n Usage: \"clan_check -f <phylip formatted tree file> -c <clan file> \"\n\n\tWhere: <phylip formatted tree file> is a phylip formatted file of trees to be assessed\n\t<clan file> is a file lists of taxa in each line (space seperated) that belong to the clan to be tested\n\n");
        exit(1);
        break;
      }
    }
	if(fflag==0 || cflag==0) 
		{
		printf ("ERROR: You must pass this software with two input files.\n");
		exit(1);
		}	


	if((treefile = fopen(treefilename, "r")) == NULL)   /* check to see if the file is there */
	    {                          /* Open the fundamental tree file */
	    fprintf(stderr, "Error: Cannot open tree file %s\n", treefilename);
	    exit(1);
	    }
	if((cladefile = fopen(cladefilename, "r")) == NULL)   /* check to see if the file is there */
	    {                          /* Open the fundamental tree file */
	    fprintf(stderr, "Error: Cannot open clan file %s\n", cladefilename);
	    exit(1);
	    }
	outfilename[0] = '\0';
	strcpy(outfilename, treefilename);
	strcat(outfilename, ".scores.txt");
	if((outfile = fopen(outfilename, "w")) == NULL)   /* check to see if the file is there */
	    {                          /* Open the fundamental tree file */
	    fprintf(stderr, "Error: Cannot open output file %s\n", outfilename);
	    exit(1);
	    }
	fprintf(outfile, "Tree number\tsize");


    /* read in how many lines (clades) are defined in the clade file so we can assign the arrays */
    while(!feof(cladefile))
	    {
	    i=0; j=0;
	    while(!feof(cladefile) && (c=getc(cladefile)) != '\n' && c != '\r') i++;
	    if(i>linelength) linelength =i;
	    numclades++;
	    }
	numclades--;
	printf("number of clans to check = %d\n", numclades);
	for(i=0; i<numclades; i++) fprintf(outfile, "\tClan %d", i+1);
	fprintf(outfile, "\tboot_average\tboot_max\tboot_min\tboot_stdev\n");

	rewind(cladefile);

	/* define the clade array and read in the clades */
	clades=malloc(numclades*sizeof(char*));
	for(i=0; i<numclades; i++)
		{	
		clades[i]=malloc((linelength+2)*sizeof(char));
		clades[i][0]='\0';
		}
	i=0;
	/* read in the clades */
    while(!feof(cladefile))
	    {
	    j=0;
	    while(!feof(cladefile) && (c=getc(cladefile)) != '\n' && c != '\r') 
	    	{
	    	if(i<numclades)
	    		{	
		    	clades[i][j]=c;
		    	j++;
		    	}
	    	}
	    if(i<numclades) {
	    	clades[i][j]='\0';
	    	/*printf("clade = %s\n", clades[i]);*/
	    	}
	    i++;
	    }
	fclose(cladefile);

	/* NOW count all the trees */
   while(!feof(treefile))
	    {
	    i=0; j=0;
	    while(!feof(treefile) && (c=getc(treefile)) != ';') i++;
	    if(i>treelength) treelength =i;
	    numtrees++;
	    }
	numtrees--;
	printf("number of trees to check = %d\n",numtrees);
	rewind(treefile);

	/* define the trees array */
	trees=malloc(numtrees*sizeof(char*));
	for(i=0; i<numtrees; i++)
		{
		trees[i]=malloc((treelength+2)*sizeof(char));
		trees[i][0]='\0';
		}
	tree=malloc((treelength*3)*sizeof(char));
	tree[0] = '\0';
	/* read in the trees */
	i=0;
    while(!feof(treefile))
	    {
	    j=0;
	    while(!feof(treefile) && (c=getc(treefile)) != ';') 
	    	{
	    	if(i<numtrees && c != '\n' && c != '\r')
	    		{	
		    	trees[i][j]=c;
		    	j++;
		    	}	
	    	}
	    if(i<numtrees) 
	    	{
	    	trees[i][j]=';';
	    	trees[i][j+1]= '\0';
	    	/*printf("tree = %s\n", trees[i]);*/
	    	}	
	    i++;
	    }
	fclose(treefile);

	constraint=malloc((treelength+2)*sizeof(char));
	constraint[0] = '\0';

	for(k=0; k<numtrees; k++)
		{
		/*printf("k=%d\n", k);*/
		fprintf(outfile, "Tree %d", k+1);

		i=0; m=0;
		while(trees[k][i] != ';')
			{
			switch(trees[k][i])
				{
				case '(':
				case ',':
					tree[m]=' '; m++;
					tree[m]=trees[k][i]; m++; i++;
					tree[m]=' '; m++;					
					break;
				case ')':
				case ':':

					tree[m]=' '; m++;
					tree[m]=trees[k][i]; m++; i++;
					tree[m]=' '; m++;	
					while(trees[k][i] != ')' && trees[k][i] != '(' && trees[k][i] != ',' && trees[k][i] != ':' && trees[k][i] != ';') i++;				
					break;
				default:
					tree[m] = trees[k][i];
					i++; m++;
					break;
				}
			}
		tree[m]=';';
		tree[m+1]='\0';


		unroottree(tree); /* unroot tree if necessary */
		/*printf("modified tree =:%s\n", tree);*/
		/*identify all the taxa in the tree */
		i=0; m=0; numtaxa=0;
		while(tree[i] != ';')
			{
			switch(tree[i])
				{
				case '(':
				case ',':
				case ' ':
					i++;
					break;
				case ':':
					while(tree[i] != '(' && tree[i] != ',' && tree[i] != ')'  && tree[i] != ';') i++;
					break;
				case ')':
					while(tree[i] != '(' && tree[i] != ',' && tree[i] != ';' && tree[i] != ':' ) i++;
					break;
				default:
					l=0;

					while(tree[i] != '(' && tree[i] != ',' && tree[i] != ')'  && tree[i] != ':' && tree[i] != ' ') 
						{
						taxa[numtaxa][l]= tree[i];
						i++; l++;
						}
					taxa[numtaxa][l] = '\0';
					numtaxa++;
					break;
				}

			}
		/*printf("number of taxa in tree are %d\n", numtaxa); */
	/*	for(i=0; i<numtaxa; i++) printf("taxa %d = >%s<\n", i, taxa[i]);*/

 		/* go through all the clades from the clades file */
		string[0]='\0';
		fprintf(outfile, "\t%d", numtaxa);
		for(n=0; n<numclades; n++)
			{	
			/*printf("Checking Clade %d\n", n); */
			/* identify the taxa from this clade that are actually in the tree */
			strcpy(string, clades[n]);
			numintree=0;
			token = strtok(string, " "); /* get the first species ID from the Clade definition */
			j=0;
			while(token != NULL )
				{	
				for(i=0; i<numtaxa; i++)
					{	
					if(strcmp(token, taxa[i]) == 0)
						{	
						clanlist[numintree][0] = '\0';
						strcpy(clanlist[numintree], token);
						/*printf("copying %s\n", clanlist[numintree]);*/
						numintree++;
						i=numtaxa;
						}
					}
				token = strtok(NULL, " ");
				} /* clanlist now contains a list of all the taxa that are in both the clade and the tree */
			foundclade=TRUE;
			if(numintree > 1) /* there is no point in looking for clades of taxa if only onw (or none) of them are in the tree */
		    	{
		    	if((numtaxa - numintree) > 1) /* If the tree is ONLY made up of taxa from the clan, then by default the clan is there and we don;t need tot test any further */
		    		{	
			    	foundclade=FALSE;
					/* Extract all the clans from the tree */
					i=0;
					/*printf("tree = %s\n", tree); */
					while(tree[i] != ';' && !foundclade)
						{
						if( tree[i] == '(' && i !=0)
							{
							  /* identify all the taxa IDs in this split */
							  g=i; j=0;
							  constraint[j] = '('; j++; constraint[j] = ' ';  j++;
							   l=1; g++; numinconstraint=0; clann_size=0;
							  while((l != 0 || tree[g-1] != ')') && tree[g] != ';' && !foundclade)   /* CHANGED RECENTLY FROM while(l != 0 && tree[k-1] != ')' && tree[k] != ';' ) */
							    {
							    switch(tree[g])
							      {
							      case '(':
							        l++;
							        g++;
							        break;
							      case ')':
							        l--;
							        g++;
							        break;
							      case ':':
							        while(tree[g] != '(' && tree[g] != ')' && tree[g] != ',' && tree[g] != ';' ) g++;
							        break;
							      case ',':
							      case ' ':
							        g++;
							        break;
							      default:
							      	tmpconstraint[0]='\0'; p=0;
							        while(tree[g] != ',' && tree[g] != '(' && tree[g] != ')' && tree[g] != ':' )
							            {
							            constraint[j] = tree[g];
							            if(tree[g] != ' ')
							            	{	
							            	tmpconstraint[p] = tree[g];
							            	p++;
							            	}
							            g++; j++;  
							            }
							        tmpconstraint[p]='\0';
							        clann_size++;
							     /*   printf("tmpconstraint=%s\n", tmpconstraint); */
							        for(p=0; p<numintree; p++)
								        {
								       /* printf("compare %s with %s -> result:%d\n", clanlist[p], tmpconstraint, strcmp(clanlist[p], tmpconstraint)); */
								        if(strcmp(clanlist[p], tmpconstraint) == 0)
								        	{
								        	numinconstraint++;
								        	}
								        }
							        constraint[j] = ' '; j++;
							        constraint[j] = ','; j++;
							        constraint[j] = ' '; j++;
							        break;
							      }
							    }
							  constraint[j-1] = ' '; /* overwrites the extra comma at the end */

							  constraint[j] = ')'; j++;
							  constraint[j] = '\0';
							  

							  /* compare this constraint to the currently defined clade from the file [ in the array cladelist ] */
							  foundclade=FALSE;
							 
							  /* if the number of taxa in or outside this constraint is less or more then the number of taxa in the clade list, then there is no point checking */
							  if(clann_size == numintree && clann_size == numinconstraint)
							  	{
							  /*	printf("constraint = %s\n", constraint);
							  	printf("checking within\n");	*/
							  	/* check to see if the taxa are the same inside the constraint as in the clade */
							  	foundtaxa=TRUE;
							  	
							  	for(l=0; l<numintree; l++) /*for all taxa in the taxalist */
							  		{
							  		string[0]=' ';
							  		string[1]='\0';
							  		strcat(string, clanlist[l]);
							  		strcat(string, " ");
							  	/*	printf("looking for \"%s\" in %s\n", string, constraint); */
							  		
							  		if(strstr(constraint,string) == NULL)
							  			{	
							  		/*	printf("didn't find it\n"); */
							  			foundtaxa=FALSE;
							  			}
							  		else
							  			{
							  		/*	printf("found it\n");	*/
							  			}
							  		}
							  	if(foundtaxa == TRUE) foundclade = TRUE;
							  	}
							
							   if(!foundclade && (numtaxa-clann_size) == numintree && numinconstraint == 0)
							  	{	
							  	/* Check to see if the taxa outside the constraint are the same as in the clade */
							/*  	printf("Found outside clan\n"); */
								foundclade = TRUE; /* this is because if k=0, then none of the clade taxa were in the constaint, and if the number of remaining taxa in the tree == number of taxa in clade, then they must be all the taxa we are looking for */
							  	}
							
					/*		printf("num in constraint = %d\tnum taxa = %d\tnumintree = %d\tClann_size=%d\n", numinconstraint, numtaxa, numintree, clann_size ); */
							  		 						  

							}
						i++;
						}
					
					if(foundclade) 
				  		{
				  			fprintf(outfile, "\t1");
				  			/*printf("found %s\n", clades[n]);*/
				  		}
					else 
				  		{
				  		fprintf(outfile, "\t0");
				  		/*printf("did not find %s\n", clades[n]);*/
				  		}
				  	}
				else
					{	
					fprintf(outfile, "\t1+"); 
					/* The tree is entirely made up of taxa from the lcan being tested, so be default it passes. */
					}
			  	}
			else /* we did not find enough taxa in to test this Clan */
				{	
				fprintf(outfile, "\t?");

				}
			g=0;

			}
		/* assess bootstrap supports */
		assess_bootstraps( trees[k], &boot_average, &boot_max, &boot_min, &boot_stdev);
		fprintf(outfile, "\t%f\t%f\t%f\t%f", boot_average, boot_max, boot_min, boot_stdev);
		fprintf(outfile, "\n");
		}
	printf("finished\n");
	fclose(outfile);
	/* free up all memory */

	for(i=0; i<numclades; i++)
		free(clades[i]);
	free(clades);
	for(i=0; i<numtrees; i++)
		free(trees[i]);
	free(trees);
	free(tree);
	free(constraint);

	}


void unroottree(char * tree)
    {
    int i=0, j=0, k=0, l=0, m=0, basecount = 0, parentheses=0;
    int foundopen = FALSE, foundclose = FALSE;
  float del_nodelen = 0;
  char length[100], restof[400000];
  
  restof[0] = '\0';
  length[0] = '\0';
  /* scan through the tree counting the number of taxa/nodes at the base (for it to be unrooted there should be at least three) */
    while(tree[i] != ';')
        {
        switch(tree[i])
            {
            case '(':
                    parentheses++;
                    i++;
                    break;
            case ')':
                    parentheses--;
                    i++;
                    break;
            case ',':
                    if(parentheses == 1)
                        {
                        basecount++;
                        }
                    i++;
                    break;
            default:
                    i++;
                    break;
            }
        }
        
    if(basecount <2)  /* if the base of the tree is rooted */
        {
        i=0;
        parentheses = 0;
        while(tree[i] != ';')  /* delete the two parentheses to make the tree unrooted */
            {
            switch(tree[i])
                {
                case '(':
                        parentheses++;
                        if(parentheses == 2 && !foundopen)
                            {
                            tree[i] = '^';
                            foundopen = TRUE;
                            }
                        i++;
                        break;
                case ')':
                        if(parentheses == 2 && !foundclose)
                            {
                            tree[i] = '^';
              i++;
              while(tree[i] != ')' && tree[i] != '(' && tree[i] != ',' && tree[i] != ';' && tree[i] != ':')
                {
                tree[i] = '^';
                i++;
                }
                            if(tree[i] == ':')
                                {
                k=0;
                length[0] = '\0';
                                while(tree[i] != ')' && tree[i] != '(' && tree[i] != ',' && tree[i] != ';')
                                    {
                  if(tree[i] != ':')
                    {
                    length[k] = tree[i];
                    k++;
                    }
                  tree[i] = '^';
                                    i++; 
                                    }
                length[k] = '\0';
                                }
              if(length[0] != '\0') /* we have a branch length on the internal branch, so we need to add it to the branch length of the component that is to the direct right of this closing parenthesis */
                {
                del_nodelen = atof(length);
                k=i+1; /* This should be whatever is after the ',' which should be the next compoonent */
                if(tree[k] == '(') /* we need to find the end of this clade and add the value there */
                  {
                  l=1; k++;
                  while((l != 0 || tree[k-1] != ')') && tree[k] != ';' )   /* CHANGED RECENTLY FROM while(l != 0 && tree[k-1] != ')' && tree[k] != ';' ) */
                    {
                    switch(tree[k])
                      {
                      case '(':
                        l++;
                        k++;
                        break;
                      case ')':
                        l--;
                        k++;
                        break;
                      default:
                        k++;
                        break;
                      }
                    }
                  k--; /* k now points to the closing bracket */
                  /* read in the length attached to this partenthsis */
                  k++;
                  while(tree[k] != ')' && tree[k] != '(' && tree[k] != ',' && tree[k] != ';' && tree[k] != ':') k++; /* k now points to the ":" at the start of the length (if there is a length) */
                  }
                else
                  {
                  while(tree[k] != ')' && tree[k] != '(' && tree[k] != ',' && tree[k] != ';' && tree[k] != ':') k++; /* k now points to the ":" at the start of the length (if there is a length) */
                  }
                if(tree[k] == ':') /* there is length attached to this */
                  {
                  m=k+1;
                  length[0] = '\0'; l=0;
                  while(tree[m] != ')' && tree[m] != '(' && tree[m] != ',' && tree[m] != ';')
                    {
                    length[l] = tree[m];
                    l++; m++;
                    }
                  length[l] = '\0';
                  del_nodelen += atof(length);
                  
                  }
                else
                  m=k;
                /* now add del_nodelen to this point in the tree */
                 l=0;
                while(tree[m] != ';' && tree[m] != '\0')
                  {
                  restof[l] = tree[m];
                  m++; l++;
                  }
                restof[l] = ';';
                restof[l+1] = '\0';
                if(tree[k] == ':')
                  tree[k] = '\0';
                else
                  {
                  tree[k] = '\0';
                  }
                length[0] = '\0';
                sprintf(length, ":%f", del_nodelen);
                strcat(tree, length);
                strcat(tree, restof);
                }
                            foundclose = TRUE;
                            }
                        i++;
                        parentheses--;
                        break;
                default:
                        i++;
                        break;
                }
            }
                        
        /* scan through the string shifting up the characters to take into account those parentheses that have been deleted */
        i=0; j=0;
        while(tree[j] != ';')
            {
            if(tree[j] == '^')
                {
                while(tree[j] == '^')
                    j++;
                if(i!= j)tree[i] = tree[j];
                i++; j++;
                }
            else
                {
                if(i!=j)tree[i] = tree[j];
                i++;j++;
                }
            }
        tree[i] = tree[j];
        tree[i+1] = '\0';
        
        }
    }

void assess_bootstraps(char * tree, float * average, float * max, float * min, float * stdev )
    {

    int i=0, j=0, num_branches=0, boot_num=0;
    float *boots, sum=0;
    char number[100];

    /* step 1 count how many branches in the tree */
    while(tree[i] != ';')
    	{
    	if(tree[i] == ')')
    		{
    		num_branches++;
    		}
    	i++;
    	}
    num_branches--; /* we want to ignore the last parenthesis */
    i=0;
   /* printf("tree= %s\nnum_branches = %d\n",tree, num_branches); */


    /* step 2 assign the array to hold the bootstrap supports */
    boots=malloc(num_branches*sizeof(float));
    for(i=0; i<num_branches; i++)
    	boots[i]=0;
    i=0;

    /* step 3 scan tree and record the bootstraps into the boots array */
    while(tree[i] != ';' && boot_num < num_branches)
    	{
    	if(tree[i] != ')') i++;
    	else
    		{
    		i++;
    		j=0;
    		while(tree[i] != ',' && tree[i] != ':' && tree[i] != ')' && tree[i] != ';')
    			{
    			number[j] = tree[i];
    			j++; i++;
    			} 
    		number[j] = '\0';
    	/*	printf("j=%d\t%s\t%f\n", j, number, atof(number)); */
    		boots[boot_num]=atof(number);
    		boot_num++;
    		}
    	}
  /*  for(i=0; i<num_branches; i++) printf ("%f\t", boots[i]);
    printf("\n"); */

    /* calculate mean, max, min and stdev of bootstraps of the tree */
    *max=0; *min=100;
    for(i=0; i<num_branches; i++)
    	{	
    	sum=sum+boots[i];
    	if(boots[i] < *min) *min=boots[i];
    	if(boots[i] > *max) *max=boots[i];
    	}
    *average=sum/(float)num_branches;

    *stdev=0;
    for(i=0; i<num_branches; i++)
    	{	
    	*stdev=*stdev+pow(fabs(boots[i]-*average),2);
    	}
    *stdev=sqrt(*stdev/num_branches);

    free(boots);
    }

