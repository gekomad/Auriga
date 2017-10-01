<html>
 <head>
  <meta charset="UTF-8">
  <meta name="robots" content="noindex">
  <title>Inside Auriga</title>
<link rel="stylesheet"  href="../css/css1.css" type="text/css"/>
<link rel="stylesheet" href="../css/layout.css" type="text/css" />

 </head>
 <body>
<?php include_once("../analyticstracking.php");?>
<?php include '../menu.php';?>
<div class="container2">
<section class="container">
          <hgroup>
            <h1>Inside Auriga</h1> 
          </hgroup>

        <div class="row">
          <section class="col-xs-12 col-sm-6 col-md-6">
			
            <section>             
			
                <h2>Work on your cluster</h2>
				<p>You can use Auriga outside of this site, in this page you will see how to configure a cluster with 3 machines (workers) 192.168.1.100, 192.168.1.101, 192.168.1.102 (you can use all the machines you want), on the three machines <a href="download.php">Download</a> auriga and configure the environment variable <code>$AURIGA_ROOT</code>
</p>	
                <h3>create a Perft (command line)</h3>
				<p>
                on 192.168.1.100 launch <pre>$AURIGA_ROOT/auriga --generate-ini $AURIGA_ROOT Ntask "FEN" DEPTH</pre>
				example create a perft with 100 tasks and depth 10:
				<pre>$AURIGA_ROOT/auriga --generate-ini $AURIGA_ROOT 100 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 10</pre>
<pre>
Auriga 0.1.x - Distributed and collaborative Perft system by Giuseppe Cannella
version 64-bit compiled Nov 10 2015 with GNU GCC 5.1.0
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>

Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:44 Fen: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:45 depth: 10
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:46 tot Ntask: 100
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:47 tot effectiveNtask: 10
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:48 tot Fen: 400
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:77 Generated file /home/geko/auriga_root/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz
X799GCE65-CDD6-43BA-G7D3-18AG9C4833D6X</pre>

799GCE65-CDD6-43BA-G7D3-18AG9C4833D6 is the uuid for the perft just created<br><br>

<pre>
$ ls -l $AURIGA_ROOT/data<br>  drwxr-xr-x 2 geko geko 4096 nov 12 13:36 799GCE65-CDD6-43BA-G7D3-18AG9C4833D6</pre>
<pre>
$ ls -l $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6
 -rw-r--r-- 1 geko geko  609 nov 12 13:36 799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz
 -rw-r--r-- 1 geko geko  760 nov 12 13:36 perft_tasks.txt
 -rw-r--r-- 1 geko geko  100 nov 12 13:36 perft.txt
 -rw-r--r-- 1 geko geko 1962 nov 12 13:36 task_fens.txt
</pre>

<pre>zmore $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz</pre>
<pre>
#auriga ini file - AUTO-GENERATED FILE - DO NOT EDIT

[perft]
perft_uuid=799GCE65-CDD6-43BA-G7D3-18AG9C4833D6
fen=rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
depth=10
[task]
task_uuid=2AD20F3E-C861-E557-250E-0F2871309DGG
depth=8
fen=rnbqkbnr/ppppppp1/8/7p/7P/8/PPPPPPP1/RNBQKBNR w KQkq h6 0 1
fen=rnbqkbnr/ppppppp1/8/7p/2P5/8/PP1PPPPP/RNBQKBNR w KQkq h6 0 1
fen=rnbqkbnr/ppppppp1/8/7p/8/5P2/PPPPP1PP/RNBQKBNR w KQkq h6 0 1
fen=rnbqkbnr/ppppppp1/8/7p/8/P7/1PPPPPPP/RNBQKBNR w KQkq h6 0 1
[task]
..
..
#END FILE

</pre>
creates the directory on the workers:
<pre>
ssh user@192.168.1.101 "mkdir $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6"
ssh user@192.168.1.102 "mkdir $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6"
</pre>
send the .gz file on the workers:
<pre>
scp $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz user@192.168.1.101:$AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6
scp $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz user@192.168.1.102:$AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6
</pre>
remove the host entry in worker file (example $AURIGA_ROOT/worker/stockfish.worker.ini) 
<br>from
<pre>host=cinnamonchess.altervista.org/auriga</pre>
to
<pre>host=</pre>

on 3 workers launch the command (replace stockfish with your engine)
<pre>
$AURIGA_ROOT/auriga --task $AURIGA_ROOT stockfish 6EDBD9C7-B1B5-C402-FEDD-1CE33A016572  2AD20F3E-C861-E557-250E-0F2871309DGG

</pre>
in windows machine launch
<pre>
%AURIGA_ROOT%\auriga.exe --task %AURIGA_ROOT% stockfish 6EDBD9C7-B1B5-C402-FEDD-1CE33A016572  2AD20F3E-C861-E557-250E-0F2871309DGG
</pre>

at the end of the work in <code>$AURIGA_ROOT/data/6EDBD9C7-B1B5-C402-FEDD-1CE33A016572/results.log</code> there is the result of calculation for the task 2AD20F3E-C861-E557-250E-0F2871309DGG, repeat last command for all uuid_task.


</p>
</section>        
</section>        </div>

</section>

<?php include '../footer.html'; ?>     
</div>
 </body>
</html>
