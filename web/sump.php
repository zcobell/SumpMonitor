<?php

echo "<html>";
echo "<head>";
echo "<title>Sump Monitor</title>";
echo "<script language=\"JavaScript\" type=\"text/javascript\" src=\"http://ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js\"></script>";
echo "<script language=\"JavaScript\" type=\"text/javascript\" src=\"https://code.highcharts.com/highcharts.js\"></script>";
echo "<script language=\"JavaScript\" type=\"text/javascript\" src=\"https://code.highcharts.com/modules/exporting.js\"></script>";
echo "<script language=\"JavaScript\" type=\"text/javascript\" src=\"sump.js\"></script>";
echo "</head>\n";
echo "<script language=\"Javascript\">\n";

// Create connection
$con=mysqli_connect("localhost","***REMOVED***","***REMOVED***","***REMOVED***");
 
// Check connection
if (mysqli_connect_errno())
{
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}


$startdate=new DateTime();
$startdate->add(DateInterval::createFromDateString('yesterday'));
$startdate->setTime(0,0,0);
$enddate=new DateTime();
$enddate->setTime(23,59,59);

// This SQL statement selects ALL from the table 'Locations'
if(isset($_GET["startdate"]))
{
    if($_GET["startdate"]!=="")
    {
        $startdate=DateTime::createFromFormat('Y-m-d',$_GET["startdate"]);
        $startdate->setTime(0,0,0);
        if(isset($_GET["enddate"]))
        {    
            $enddate=DateTime::createFromFormat('Y-m-d',$_GET["enddate"]);
            $enddate->setTime(23,59,59);
        }
        $startdatestring=$startdate->format('Y-m-d H:i:s');
        $enddatestring=$enddate->format('Y-m-d H:i:s');
        $sql = "SELECT SQL_NO_CACHE * FROM sumpData WHERE time >= '".$startdatestring."' and time <= '".$enddatestring."' ORDER BY time";
    }
    else
        $sql = "SELECT SQL_NO_CACHE * FROM sumpData WHERE time >= DATE_SUB(NOW(),INTERVAL 24 HOUR) ORDER BY time"; 
}
else
    $sql = "SELECT SQL_NO_CACHE * FROM sumpData WHERE time >= DATE_SUB(NOW(),INTERVAL 24 HOUR) ORDER BY time";

// Check if there are results
if ($result = mysqli_query($con, $sql))
{
    // If so, then create a results array and a temporary one
    // to hold the data
    $resultArray = array();
    $tempArray = array();
 
    // Loop through each row in the result set
    while($row = $result->fetch_object())
    {
        // Add each row into our results array
        $tempArray = $row;
        array_push($resultArray, $tempArray);
    }
 
    // Finally, encode the array to JSON and output the results
    echo "var sumpdata=";
    echo json_encode($resultArray);
}

echo "\n</script>\n";
 
// Close connections
mysqli_close($con);

if(isset($_GET["startdate"]))
    echo "<body onload=\"getSumpData(1);\">";
else
    echo "<body onload=\"getSumpData(0);\">";

echo "<div id=\"waterlevel\" style=\"min-width: 310px; height: 400px; margin: 0 auto\"></div>";
echo "<div id=\"stats\"></div>";
echo "<div id=\"form\">";
echo "<form method=\"get\">";
echo "    <p>";
echo "        <label for=\"startdate\">Start Date</label>";
echo "        <input type=\"date\" name=\"startdate\" id=\"startdate\" value=\"".$startdate->format('Y-m-d')."\">";
echo "        <label for=\"endate\">&nbsp;&nbsp;End Date</label>";
echo "        <input type=\"date\" name=\"enddate\" id=\"enddate\" value=\"".$enddate->format('Y-m-d')."\">";
echo "        <input type=\"submit\" name=\"submit\" id=\"submit\" value=\"Query\">";
echo "    </p>";
echo "</div>";
echo "</body>";
echo "</html>";

?>
