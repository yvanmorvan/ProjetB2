<!DOCTYPE html>

<!--
    * PROJET B2 -- Colombus *
    *
    * 158243 SELVA Florian  *
    * 160794 NAIBO Gaël     *
    * 161076 MIARD Hugo     *
-->

<html lang="en">
<head>
    <meta charset="utf-8" />
	<meta name="viewport" content="width=device-width"/>
    <title>Colombus</title>

    <meta name="robots" content="index, follow" />
    <meta name="keywords" content="Colombus" />
    <meta name="description" content="Colombus, a brand new powerful search engine. We'll enhance your cereals !" />

    <!-- STYLES -->
    <link href="styles/style.css" rel="stylesheet" type="text/css" />
    <!-- SCRIPTS -->
	<script type="text/javascript" src="js/jquery-1.10.2.min.js"></script>
    <script type="text/javascript" src="js/colombus.js"></script>

</head>

<body>

    <div id="container">

        <div id="research-wrapper">
            <div id="branding">Colombus</div>

            <form id="enter-rs" action="client.php" method="POST">
            
                <input type="text" name="research" id="rs" />
                <input type="submit" name="send-rs" id="send-rs" value="Find for me"/>

            </form>
            <div id="bg-in" class="bg"></div>
        </div>
        <div id="bg-out" class="bg"></div>
    </div>

</body>

</html>