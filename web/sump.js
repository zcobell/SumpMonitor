
var waterlevelchart;
var cycletimechart;
var floatData;
var waterData;
var cycleTime;

function pad(num, size) {
    var s = "000000000" + num;
    return s.substr(s.length-size);
}

function getSumpData(isQuery)
{
    parseJSONData(sumpdata,isQuery);
}

Date.prototype.stdTimezoneOffset = function() {
    var jan = new Date(this.getFullYear(), 0, 1);
    var jul = new Date(this.getFullYear(), 6, 1);
    return Math.max(jan.getTimezoneOffset(), jul.getTimezoneOffset());
}

Date.prototype.dst = function() {
    return this.getTimezoneOffset() < this.stdTimezoneOffset();
}

function parseJSONData(data,isQuery)
{
    var i;
    var json_date          = new Array();
    var json_float         = new Array();
    var json_waterlevel    = new Array();
    var json_datevar       = new Array();
    var temp;

    for(i=0;i<data.length;i++)
    {
        temp = data[i].time.split(" ");
        tempDate = temp[0];
        tempTime = temp[1];

        tempDate2 = tempDate.split("-");
        year = Number(tempDate2[0]);
        month = Number(tempDate2[1])-1;
        day = Number(tempDate2[2]);

        tempTime2 = tempTime.split(":");
        hour = Number(tempTime2[0]);
        minute = Number(tempTime2[1]);
        second = Number(tempTime2[2]);

        json_date[i] = Date.UTC(year,month,day,hour,minute,second);
        json_datevar[i] = new Date(year,month,day,hour,minute,second);

        json_waterlevel[i] = Number(data[i].waterlevel);
        json_float[i]      = Number(data[i].floatstatus);

    }

    plot(json_date,json_datevar,json_float,json_waterlevel,isQuery);

}

function plot(m_date,m_datevar,m_f,m_wl,isQuery)
{
    var i;
    var nRuns;
    var gallons;
    var dt;
    var sumpPitDiameter = 18.5; //inches
    var oneHour = 60*60*1000;
    var maxwl,minwl;
    var lastCycle;
    var tempDate;
    var timestring;
    var lastData;
    
    floatData = new Array();
    waterData = new Array();
    cycleTime = new Array();

    var plotToolTip = {
        formatter: function() {
        return '<b>'+ this.series.name +'</b><br/>'+Highcharts.dateFormat('%b %e, %Y %l:%M%p', this.x) +': '+ this.y.toFixed(2) +' ';}
    };

    nRuns = 0;
    gallons = 0;
    dt = 0;
    maxwl = -999;
    minwl = 999;

    for(i=0;i<m_date.length;i++) {
        floatData[i] = new Array();
        waterData[i] = new Array();
        cycleTime[i] = new Array();
        
        floatData[i][0] = m_date[i];
        waterData[i][0] = m_date[i];
        lastData = m_datevar[i];
        
        floatData[i][1] = m_f[i];
        waterData[i][1] = m_wl[i];

        if(waterData[i][1]>maxwl)
            maxwl = waterData[i][1];

        if(waterData[i][1]<minwl)
            minwl = waterData[i][1];

        if(i>0) {
            if(waterData[i-1][1]-waterData[i][1] > 1.0) {
                nRuns = nRuns + 1;
                gallons = gallons + ( (waterData[i-1][1]-waterData[i][1]) * Math.PI * (sumpPitDiameter/2.0) * (sumpPitDiameter/2.0) ) / 231.0;
                tempDate = new Date(m_date[i]);
                if(nRuns>1){ 
                    dt = dt + (tempDate.getTime()-lastCycle.getTime())/oneHour;
                    cycleTime[nRuns-2][0] = tempDate.getTime();
                    cycleTime[nRuns-2][1] = Math.round(((tempDate.getTime()-lastCycle.getTime())/oneHour)*100.0)/100.0;
                }
                lastCycle = tempDate;
            }
        }

    }

    if(nRuns>1){
        dt = dt / (nRuns-1);
        cycleTime[0][0] = waterData[0][0];
        cycleTime[nRuns-2][0] = waterData[m_date.length-1][0];
    }else{
        dt = 0;
    }

    if(dt>1.0)
        timestring="hours";
    else {
        timestring="minutes";
        dt = dt * 60;
    }

    var timezoneOffset = new Date().getTimezoneOffset();    
    var today = new Date();
    if ( today.dst() )
        var mountainTime = 360.0;
    else
        var mountainTime = 420.0;

    lastData.setMinutes(lastData.getMinutes()+(mountainTime-timezoneOffset));

    var lastDataYear    = lastData.getYear()+1900;
    var lastDataMonth   = pad(lastData.getMonth()+1,2);
    var lastDataDay     = pad(lastData.getDate(),2);
    var lastDataHour    = pad(lastData.getHours(),2);
    var lastDataMinute  = pad(lastData.getMinutes(),2);
    var lastDataSecond  = pad(lastData.getSeconds(),2);
    var ampm;
    if(lastData.getHours()>12) {
        lastDataHour = pad(lastData.getHours()-12,2);
        ampm = "PM";
    } else {
        lastDataHour = pad(lastData.getHours(),2);
        ampm = "AM";
    }

    var isDown;
    var timezoneDiff = -(timezoneOffset - mountainTime)*60;
    now = new Date();

    if((now.getTime()-lastData.getTime())/1000 > (10*60))
        isDown = 1;
    else
        isDown = 0;

    var tableHTML ="<table> "+
                   "<tr>" + 
                       "<td align=\"right\"><b>Number of Cycles:</b></td><td>"+nRuns+"</td>"+
                   "</tr>"+
                   "<tr>"+
                       "<td align=\"right\"><b>Average Time Between Cycles:</b></td><td>"+Math.round(dt*100)/100+" "+timestring+"</td>"+
                   "</tr>"+
                   "<tr>"+
                       "<td align=\"right\"><b>Pumping Volume For Date Range Shown:</b></td><td>"+Math.round(gallons*100)/100+" gallons</td>"+
                   "</tr>"+
                   "<tr>"+
                       "<td align=\"right\"><b>Maximum water level: </b></td><td>"+Math.round(maxwl*100)/100+" inches</td>"+
                   "</tr>"+
                   "<tr>"+
                       "<td align=\"right\"><b>Minimum water level: </b></td><td>"+Math.round(minwl*100)/100+" inches</td>"+
                   "</tr>";
    if(isQuery==0) {
       tableHTML = tableHTML +
                   "<tr>"+
                       "<td align=\"right\"><b>Last data received: </b></td><td>"+lastDataMonth+"/"+lastDataDay+"/"+lastDataYear+" "+
                                  lastDataHour+":"+lastDataMinute+":"+lastDataSecond+" "+ampm+"</td>"+
                   "</tr>";
        if(isDown==0)
            tableHTML = tableHTML + "<tr><td align=\"right\"><b>Monitor Status: </b></td><td> <b><font color=\"green\">ACTIVE</font></b></td></tr>";
        else
            tableHTML = tableHTML + "<tr><td align=\"right\"><b>Monitor Status: </b></td><td> <b><font color=\"red\">DOWN</font></b></td></tr>";
    }

    tableHTML = tableHTML + "</table>";

    document.getElementById('stats').innerHTML = tableHTML;                                                


    showPlots();

}


function showPlots() {
    
    //...Show last 24 hrs
    var xmin = floatData[0][0];
    var xmax = floatData[floatData.length-1][0];

    var xData = { min: xmin, max: xmax, type: 'datetime', title: { text: 'Date (Mountain Time)' }, dateTimeLabelFormats: { month: '%e. %b', year: '%b' }, gridLineWidth: 1 };
    var plotOption = { series: { marker: { enabled: false }, animation: { enabled: true, duration: 1000, easing: 'linear' }, } };
    var plotToolTip = {
             formatter: function() {
                 return '<b>'+ this.series.name +'</b><br/>'+
                 Highcharts.dateFormat('%b %e, %Y %l:%M%p', this.x) +': '+ this.y.toFixed(2) +' ';
             }
    };
    
    waterlevelchart = new Highcharts.chart({
        title: {
            text: 'Sump Water Level',
            x: -20 //center
        },
        chart : { zoomType: "xy",
                  renderTo: 'waterlevel' },
        xAxis: xData,
        yAxis: { labels: {format: '{value:.2f}'}, title: { text: 'Water Level (in)' }, gridLineWidth: 1, alternateGridColor: '#EEEEEE' },
        series: [{ name: "Water Level", data: waterData, color: '#0000FF' }],
        plotOptions: { line: { marker: { enabled: false }, animation: false }}
    });

    cycletimechart = new Highcharts.chart({
        title: {
            text: 'Sump Cycle Time',
            x: -20 //center
        },
        chart : { zoomType: "xy",
                  renderTo: 'cycletime' },
        xAxis: xData,
        yAxis: { labels: {format: '{value:.2f}'}, title: { text: 'Cycle Time (hr)' }, gridLineWidth: 1, alternateGridColor: '#EEEEEE' },
        series: [{ name: "Cycle Time", data: cycleTime, color: '#0000FF' }],
        plotOptions: { line: { marker: { enabled: false }, animation: false }}
    });
    
}

function openPlot(evt, plotName) {
  var i, tabcontent, tablinks;
  tabcontent = document.getElementsByClassName("tabcontent");
  for(i=0;i<tabcontent.length;i++){
      tabcontent[i].style.display = "none";
  }

  tablinks = document.getElementsByClassName("tablinks");
  for(i=0;i<tablinks.length;i++){
      tablinks[i].className = tablinks[i].className.replace(" active", "");
  }

  document.getElementById(plotName).style.display = "block";
  evt.currentTarget.className += " active";
}

function redraw_charts() {
    waterlevelchart.destroy();
    cycletimechart.destroy();
    showPlots();
}
