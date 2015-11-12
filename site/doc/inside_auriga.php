<html>
 <head>

  <meta name="robots" content="noindex">
  <title>Inside Auriga</title>
<link rel="stylesheet"  href="../css/css1.css" type="text/css"/>
<link rel="stylesheet" href="../css/layout.css" type="text/css" />

 </head>
 <body>
<?php include '../menu.php';?>
<div class="container2">
<section class="container">
          <hgroup>
            <h1>Inside Auriga </h1> 
          </hgroup>

        <div class="row">
          <section class="col-xs-12 col-sm-6 col-md-6">
			
            <section>             
			
                <h2>Lavorare sulla tua rete</h2>
				<p>Puoi utilizzare Auriga al di fuori di questo sito.</p>	
                <h3>Creare un perft a riga di comando</h3>
				<p>
                <pre>./auriga --generate-ini $AURIGA_ROOT Ntask "FEN" DEPTH</pre>
				example:
				<pre>./auriga --generate-ini $AURIGA_ROOT 10 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 10</pre>
<pre>
Auriga 0.1.x - Distributed and collaborative Perft system by Giuseppe Cannella
version 64-bit compiled Nov 10 2015 with GNU GCC 5.1.0
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>

Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:44 Fen: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:45 depth: 10
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:46 tot Ntask: 10
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:47 tot effectiveNtask: 10
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:48 tot Fen: 20
Thu Nov 12 13:36:05 2015 INFO PerftSplitter.cpp:77 Generated file /home/geko/auriga_root/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz
X799GCE65-CDD6-43BA-G7D3-18AG9C4833D6X</pre>

799GCE65-CDD6-43BA-G7D3-18AG9C4833D6 is the uuid fot the perft appena creato<br><br>

<pre>
$ ls -l $AURIGA_ROOT/data<br>  drwxr-xr-x 2 geko geko 4096 nov 12 13:36 799GCE65-CDD6-43BA-G7D3-18AG9C4833D6</pre>
<pre>
$ ls -l $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6
 -rw-r--r-- 1 geko geko  609 nov 12 13:36 799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz
 -rw-r--r-- 1 geko geko  760 nov 12 13:36 perft_tasks.txt
 -rw-r--r-- 1 geko geko  100 nov 12 13:36 perft.txt
 -rw-r--r-- 1 geko geko 1962 nov 12 13:36 task_fens.txt
</pre>

i file *.txt possono essere cancellati o  aggiornare un aventuale DB<br><br>
<pre>zmore $AURIGA_ROOT/data/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6/799GCE65-CDD6-43BA-G7D3-18AG9C4833D6.ini.gz</pre>
<pre>
#auriga ini file - AUTO-GENERATED FILE - DO NOT EDIT

[perft]
perft_uuid=799GCE65-CDD6-43BA-G7D3-18AG9C4833D6
fen=rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
depth=10
[task]
task_uuid=2AD20F3E-C861-E557-250E-0F2871309DGG
depth=9
fen=rnbqkbnr/pppppppp/8/8/7P/8/PPPPPPP1/RNBQKBNR b KQkq h3 0 1
fen=rnbqkbnr/pppppppp/8/8/8/5P2/PPPPP1PP/RNBQKBNR b KQkq - 0 1
[task]
..
..

</pre>
				</p>
           </section>        
          </section>        </div>

</section>

<?php include '../footer.html'; ?>     
</div>
 </body>
</html>
