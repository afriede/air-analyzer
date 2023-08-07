extern String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
  <meta charset='utf-8'>
  <style>
    body {font-size:100%;}
    #main {display: table; margin: auto;  padding: 0 10px 0 10px; }
    h2 {text-align:center; }
    p { text-align:center; }
  </style>
  <script>
   function refresh(refreshPeriod)
   {
      setTimeout("location.reload(true);", refreshPeriod);
   }
   window.onload = refresh(5000);
  </script>
  <title>Air analyzer</title>
 </head>

 <body>
   <div id='main'>
     <h2>Air analyzer</h2>
     <div id='count'>
       <p>CO2  = %co2_val%ppm</p>
       <p>Temp = %temp_val%°C</p>
       <p>Hum = %hum_val%%RH</p>
       <p>PM2.5 = %pm_val%μg/m³</p>
       <p>VOXi = %voxi_val%VOC Index points</p>
       <p>NO2i = %no2i_val%NOx Index points</p>
     </div>
   </div>
 </body>
</html>
)=====";