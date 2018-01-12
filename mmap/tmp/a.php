<?php 
$value = getservbyport(137, "udp"); 
echo $value;

$value = getservbyport(80, "tcp"); 
echo $value;

?> 
