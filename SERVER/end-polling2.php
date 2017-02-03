<?php
// modulo di polling data ( tecnologia ajax )
// Autore : Sergio Guastaferro
// Descrizione : interfaccia ajax polloing
// modalità : POST
//

header('Access-Control-Allow-Origin: *');

function request() {

	$nazione =	$_GET["nazione"]; 
	$regione =	$_GET["regione"];
	$paese = 	$_GET["paese"];
	
	$path = "data/nations/" . $nazione . "/" . $regione . "/" . $paese . ".xml";
	$xml = new SimpleXMLElement($path, NULL, TRUE);
	
	
	//////////////////////////////////////////
	//	INVIAMI L'ULTIMA MISURA!!!
	///////////////////////////////////////
	 $i = count( $xml->misura ) - 1;
	 
			$temperatura = $xml->misura[$i]->temperatura;
			$umidita = $xml->misura[$i]->umidita;
			$orario = $xml->misura[$i]->orario;
			
			$id = "$nazione#$regione#$paese";
			
			//	FORMATTA I DATI IN JSON
			$out = json_encode(array("num"=>$i,"id"=>$id,"temperatura"=>$temperatura,"umidita"=>$umidita,"orario"=>$orario));
	
	
	
	return $out;
}

echo request();

?>