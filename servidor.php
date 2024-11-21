<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "nodemcu";

try {
    // Creamos la conexión a la DB
    $conn = new PDO("mysql:host=$servername;dbname=$dbname;charset=utf8", $username, $password);
    // Configuramos PDO para las excepciones en caso de error
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    if ($_SERVER['REQUEST_METHOD'] === 'POST') {
        // Recibimos los dato enviado desde NodeMCU
        $value = $_POST['estado'] ?? null;

        if ($value !== null) {
            // Insertamos los datos en la base de datos
            $stmt = $conn->prepare("INSERT INTO sensor (valor) VALUES (:valor)");
            $stmt->bindParam(':valor', $value, PDO::PARAM_STR);
            $stmt->execute();
            echo ("Datos Guardados");
        } else {
            echo ("Error: 'estado' no enviado o es nulo.");
        }
    } elseif ($_SERVER['REQUEST_METHOD'] === 'GET') {
        // Consultamos los datos de la base de datos
        $stmt = $conn->query("SELECT * FROM sensor");
        $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
        echo json_encode($data);
    }
} catch (PDOException $e) {
    echo "Error: " . $e->getMessage();
}

    // Cerramos la conexión
    $conn = null;
?>
