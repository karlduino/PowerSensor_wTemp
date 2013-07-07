#!/usr/bin/perl
# insert generic info into PowerSensor.ino

$origfile = "PowerSensor.ino";
$tmpfile = "tmp";
$insertion = "generic.txt";

if(-e $tmpfile) { die("tmp file already exists"); }
system("mv $origfile $tmpfile");
open(IN, $tmpfile) or die("Cannot read from $tmpfile");
open(OUT, ">$origfile") or die("Cannot write to $origfile");
while($line = <IN>) {
    if($line =~ /private/) {
        open(IN2, $insertion) or die("Cannot read from $insertion");
        while($line = <IN2>) {
            print OUT $line;
        }
        while($line = <IN>) {
            if($line =~ /private/) { last; }
        }
    }
    else {
        print OUT $line;
    }
}
system("rm $tmpfile");
