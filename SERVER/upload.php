<?php
// modulo di upload data
// Autore : Sergio Guastaferro
// Descrizione : interfaccia di upload misure controllore/server
// modalità : GET
//
function start(){
if ( empty($_GET['t']) or empty($_GET['u']) or empty($_GET['id']) ) { return "errore:campi vuoti"; }

$temperatura = $_GET["t"];
$umidità = $_GET["u"];

$id = $_GET["id"]; //nazione#regione#citta     # in esadecimale è 23  ->> nazione%23regione%23citta

$decoded = explode("#", $id);

$nazione = $decoded[0];
$regione = $decoded[1];
$citta = $decoded[2];

$path = "data/nations/" . $nazione . "/" . $regione . "/" . $citta . ".xml";

$xml = new SimpleXMLElement($path, NULL, TRUE);

$today = date("j F Y, g:i a");

$misura = $xml->addChild('misura');
	$misura->addChild('temperatura', $temperatura);
	$misura->addChild('umidita', $umidità);
	$misura->addChild('orario', $today); 

$xml->asXML($path);

return "$id : inviato";

	}

echo start();

?>