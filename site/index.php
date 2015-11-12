<html>
 <head>

  <meta name="robots" content="noindex">
  <title>Auriga</title>
<link rel="stylesheet"  href="css/css1.css" type="text/css"/>
<link rel="stylesheet" href="css/layout.css" type="text/css" />

 </head>
 <body>
<?php include_once("analyticstracking.php");?>
<?php include 'menu.php';?>
<div class="container2">
<section class="container">
          <hgroup>
            <h1>Auriga </h1> 
          </hgroup>

        <div class="row">
          <section class="col-xs-12 col-sm-6 col-md-6">
			
			<img src="img/logo.jpg" ALIGN="right" border="60px"/> 
            <section>
              <h2>Overview</h2>
                <p>Auriga è un sistema open source distribuito collaborativo e multi piattaforma per il calcolo della funzione <a href="https://chessprogramming.wikispaces.com/Perft">Perft</a>.
                <h3>Come funziona</h3>
                <p>L'albero viene suddiviso in tasks, ogni task sarà analizzato da una singola macchina (<b>worker</b>) e il risultato sarà inviato automaticamente a questo sito. Quando tutti i tasks saranno elaborati si avrà il risultato finale del perft.
				<h3>Setup worker</h3>
  <a href="download.php">Scaricare</a> Auriga per la tua piattaforma e copiare la cartella in quelche parte del tuo file system; 
settare la variabile di ambiente AURIGA_ROOT ad esempio su ubuntu inserisci nel file ~/.bashrc <br><br> <pre>export AURIGA_ROOT="/your/path/auriga_root"</pre>
per windows clicca <a href="https://support.microsoft.com/en-us/kb/310519">qui.</a><br><br>La cartella <i>auriga_root</i> contiene un'altra cartella denominata <i>worker</i> con le configurazioni per alcuni motori, vedi <a href="doc/worker.php">qui</a> per la configurazione del worker.
</p>

<h3>Lanciare un worker</h3>
Prima di lanciare un worker devi averlo <a href="doc/worker.php">configurato</a>.<br><br>
<p>Puoi lanciare un worker su:
<ul>
<li> un task random non acora completato, vai <a href="allperft.php">qui</a> seleziona un worker e clicca sul bottone 'Generate command for random task',
copia e incolla il comando generato sulla shell della tua macchina.</li>

<li>Tutti i task non completati di un perft - vai <a href="allperft.php">qui</a> clicca su un perft e poi su 'Generate command for uncompleted tasks' copia e incolla il comando generato sulla shell della tua macchina.</li>

<li>Un determinato task - dalla pagina precedente clicca su un task e poi su 'Generate command'  copia e incolla il comando generato sulla shell della tua macchina. </li></p>
</ul>
			
                <h2>Lavorare sulla tua rete</h2>
			
                <p>View <a href="doc/inside_auriga.php">inside auriga</a>  </p>
            


          </section>
        </div>



</section>

<?php include 'footer.html'; ?>     
</div>
 </body>
</html>
