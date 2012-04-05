#!/usr/bin/perl

$BIN = "./bin/01knapsack";
if ( !(-e $BIN && -x $BIN) )
{
	print "No executable file!!\n";
	exit;
}

$list = `ls ./inputs`;

@sp = split/\n/, $list;
@file = ();
foreach(@sp)
{
	if ( $_ =~ /(.*).in/ )
	{
#		print "$1\n";
		push( @file, $1 );
	}
}
$cnt = 1;
foreach(@file)
{
	$filename = $_;
	$filename =~ /n(\d+)[sS](\d+)/i;
	$n = $1;
	$s = $2;
	print STDERR "$n, $s\n";
	print "\n**** Testing inputfile ($filename) ****\n";
#	print "$filename\t";

#	print "Generating output for each sort...\n";
#	print "----------------------\n";
#	print "| Insertion Sort     |\n";
#	print "----------------------\n";
	if ( $n < 32 )
	{
		print `$BIN -BF ./inputs/$filename.in ./outputs/$filename.out.bf`;
	}
#	print "\n";
	
#	print "----------------------\n";
#	print "| Merge Sort         |\n";
#	print "----------------------\n";
	print `$BIN -GD ./inputs/$filename.in ./outputs/$filename.out.gd`;
#	print "\n";
	
#	print "----------------------\n";
#	print "| Heap Sort          |\n";
#	print "----------------------\n";
	print `$BIN -DP ./inputs/$filename.in ./outputs/$filename.out.dp`;
	
	
	print `$BIN -RC ./inputs/$filename.in ./outputs/$filename.out.rc`;
	print "\n\n";


	print "comparing the output results...\n";
	
	if ( $n < 32 )
	{
		print "==> BF vs. Greedy\n";
		print `diff ./outputs/$filename.out.bf ./outputs/$filename.out.gd\n`;
		
		print "==> BF vs. DP\n";
		print `diff ./outputs/$filename.out.bf ./outputs/$filename.out.dp\n`;
		
		print "==> BF vs. RC\n";
		print `diff ./outputs/$filename.out.bf ./outputs/$filename.out.rc\n`;
	}

	print "==> GD vs. DP\n";
	print `diff ./outputs/$filename.out.gd ./outputs/$filename.out.dp`;

	print "==> DP vs. RC\n";
	print `diff ./outputs/$filename.out.dp ./outputs/$filename.out.rc\n`;
	

	print STDERR "testcase $filename is finished, continue to next file?[Y|n] ";
	$c = <STDIN>;
	chomp($c);
	
	if ( $c eq "n" || $c eq "N" )
	{
		last;
	}
}
	
print STDERR "\n\n\nDelete all the generated output?[y|N] ";
$c = <STDIN>;
chomp($c);

if ( $c eq "y" || $c eq "Y" )
{
	foreach(@file)
	{
		$filename = $_;
		`rm -f ./outputs/$filename.out.*`;
	}
}
