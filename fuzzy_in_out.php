<?php 

// Recebe os parametros Luminosidade / Umidade / Temperatura do NodeMCU via Requisição GET
$luminosidade = $_GET["temp"];
$umidade = $_GET["humid"];
$temperatura = $_GET["lumin"];

// $luminosidade = 26;
// $umidade = 65;
// $temperatura = 32;

$luminosidade = intval($luminosidade);
$umidade = intval($umidade);
$temperatura = intval($temperatura);

// Executa o script em R, passando os parametros recebidos via GET
$output = exec('"C:\Program Files\R\R-3.5.3\bin\Rscript.exe" script.r '.$temperatura.' '.$umidade.' '.$luminosidade.' 2>&1');

echo $output;

$saida_fuzzy_txt = "saida.txt";
$linha = '"x"';

// Ler o txt saida.txt gerado pelo script fuzzy em R
$contents = file_get_contents($saida_fuzzy_txt);
// Remove a linha "x", do script fuzzy
$contents = str_replace($linha, '', $contents);
// Salva o txt saida.txt sem a linha "x"
file_put_contents($saida_fuzzy_txt, $contents);

// Exibe o conteudo do novo txt saida.txt
echo $contents;
?>
