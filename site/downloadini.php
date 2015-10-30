<?php header("Content-Type:text/plain");echo htmlspecialchars(file_get_contents($_GET['id'].".ini")); ?>
