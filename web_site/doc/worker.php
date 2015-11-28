<html>
 <head>
  <meta charset="UTF-8">
  <meta name="robots" content="noindex">
  <title>Worker</title>
<link rel="stylesheet"  href="../css/css1.css" type="text/css"/>
<link rel="stylesheet" href="../css/layout.css" type="text/css" />

 </head>
 <body>
<?php include_once("../analyticstracking.php");?>
<?php include '../menu.php';?>
<div class="container2">
<section class="container">
          <hgroup>
            <h1>Worker</h1> 
          </hgroup>

        <div class="row">
          <section class="col-xs-12 col-sm-6 col-md-6">			
            <section>             			             
				<p>The worker is a file in the folder <code>$AURIGA_ROOT/worker</code> and identifies the engine used on your machine to calculate the perft.</p>	
            
<pre>
#AURIGA worker ini file

<b>[auriga_server]</b>
host=server host where get and put data
port=host port

<b>[worker]</b>
#your name (will be published)
public_name=name or nick
personal_uuid=0

<b>[engine]</b>
path=full path engine

#engine output for perft result
regex_perft_moves=a regexp with (\d+) 

force_restart=boolean

instances=n' instances

<b>[setoption_name_value]</b>
#uci options
#option=value

</pre>

It is necessary set properly the entry <b>path</b> with the absolute path of the engine you want to use and the entry <b>regex_perft_moves</b> is the string that returns the engine after running a perft.
<br>Example of <b>Cinnamon</b> worker with some uci options:<br><br>
<pre>
#AURIGA worker ini file

<b>[auriga_server]</b>
host=auriga-cinnamon.rhcloud.com
port=80

<b>[worker]</b>
public_name=John Smith (Notebook Dell)
personal_uuid=0

<b>[engine]</b>
path=/home/geko/cinnamon

regex_perft_moves=<span style="color:red">Perft moves: (\d+) in .*</span>

force_restart=true

instances=1

<b>[setoption_name_value]</b>
#send to engine: setoption name PerftThreads value 4
PerftThreads=4

#send to engine: setoption name PerftHashSize value 1000
PerftHashSize=1000
</pre>

in this example will be run one process of cinnamon with 4 threads and 1 Gb hashtable. To found <b>regex_perft_moves</b> run a simple perft and creates the regex matching, must be only one (\d+) that identifies the moves calculated<br><br>
<pre>
perft 1
<span style="color:red">Perft moves: 20 in 0 seconds 1 millsec</span>
</pre>


<br>Example of <b>Crafty</b> worker, will be run 2 parallel process:<br><br>
<pre>
#AURIGA worker ini file

<b>[auriga_server]</b>
host=auriga-cinnamon.rhcloud.com
port=80

<b>[worker]</b>
public_name=Alice L.
personal_uuid=0

<b>[engine]</b>
path=/Users/geeno/engines/crafty
regex_perft_moves=<span style="color:red">total moves=(\d+)  time=.*</span>
force_restart=true
instances=2
</pre>

to found <b>regex_perft_moves</b> run a simple perft and creates the regex matching, must be only one (\d+) that identifies the moves calculated<br><br>
<pre>
White(1): xboard

tellicsnoalias set 1 Crafty v23.4 (1 cpus)
tellicsnoalias kibitz Hello from Crafty v23.4! (1 cpus)
perft 2
<span style="color:red">total moves=400  time=0.00</span>
</pre>

<br>Example of <b>Stockfish</b> worker, will be run 4 parallel process:<br><br>
<pre>
#AURIGA worker ini file

<b>[auriga_server]</b>
host=auriga-cinnamon.rhcloud.com
port=80

<b>[worker]</b>
public_name=Bob (I7)
personal_uuid=0

<b>[engine]</b>
path=c:\chess\myengines\stockfish.exe
regex_perft_moves=<span style="color:red">Nodes searched  : (\d+)\n</span>
force_restart=true
instances=4
</pre>

to found <b>regex_perft_moves</b> run a simple perft and creates the regex matching, must be only one (\d+) that identifies the moves calculated. In this case don't set <b>\r\n</b> but only <b>\n</b><br><br>
<pre>
Stockfish 6 64 by Tord Romstad, Marco Costalba and Joona Kiiski
perft 1

Position: 1/1
a2a3: 1
..
..
g1h3: 1

===========================
Total time (ms) : 1
<span style="color:red">Nodes searched  : 20</span>
Nodes/second    : 20000
</pre>


				</p>
           </section>        
          </section>        </div>

</section>

<?php include '../footer.html'; ?>     
</div>
 </body>
</html>
