

function getSumpData()
{
    parseJSONData(sumpdata);
}



function parseJSONData(data)
{
    var i;
    var json_date          = new Array();
    var json_float         = new Array();
    var json_waterlevel    = new Array();
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

        json_waterlevel[i] = Number(data[i].waterlevel);
        json_float[i]      = Number(data[i].floatstatus);

    }

    plot(json_date,json_float,json_waterlevel);

}

function plot(m_date,m_f,m_wl)
{
    var i;
    var floatData = new Array();
    var waterData = new Array();
    var nRuns;
    var gallons;
    var dt;
    var sumpPitDiameter = 18.5; //inches
    var oneHour = 60*60*1000;
    var maxwl,minwl;
    var lastCycle;
    var tempDate;

    var plotToolTip = {
        formatter: function() {
        return '<b>'+ this.series.name +'</b><br/>'+Highcharts.dateFormat('%b %e, %Y %l:%M%p', this.x) +': '+ this.y.toFixed(2) +' ';}
    };

    nRuns = 0;
    gallons = 0;
    dt = 0;
    maxwl = -999;
    minwl = 999;

    for(i=0;i<m_date.length;i++)
    {
        floatData[i] = new Array();
        waterData[i] = new Array();
        
        floatData[i][0] = m_date[i];
        waterData[i][0] = m_date[i];
        
        floatData[i][1] = m_f[i];
        waterData[i][1] = m_wl[i];

        if(waterData[i][1]>maxwl)
            maxwl = waterData[i][1];

        if(waterData[i][1]<minwl)
            minwl = waterData[i][1];

        if(i>0)
        {
            if(waterData[i-1][1]-waterData[i][1] > 1.0)
            {
                nRuns = nRuns + 1;
                gallons = gallons + ( (waterData[i-1][1]-waterData[i][1]) * Math.PI * (sumpPitDiameter/2.0) * (sumpPitDiameter/2.0) ) / 231.0;
                tempDate = new Date(m_date[i]);
                if(nRuns>1)
                    dt = dt + (tempDate.getTime()-lastCycle.getTime())/oneHour;
                lastCycle = tempDate;
            }
        }

    }

    if(nRuns>1)
        dt = dt / (nRuns-1);
    else
        dt = 0;

    document.getElementById('stats').innerHTML = "<table> "+
                                                    "<tr>" + 
                                                        "<td align=\"right\"><b>Number of Cycles:</b></td><td>"+nRuns+"</td>"+
                                                    "</tr>"+
                                                    "<tr>"+
                                                        "<td align=\"right\"><b>Average Time Between Cycles:</b></td><td>"+Math.round(dt*100)/100+" hours</td>"+
                                                    "</tr>"+
                                                    "<tr>"+
                                                        "<td align=\"right\"><b>Pumping Volume For Date Range Shown:</b></td><td>"+Math.round(gallons*100)/100+" gallons</td>"+
                                                    "</tr>"+
                                                    "<tr>"+
                                                        "<td align=\"right\"><b>Maximum water level: </b></td><td>"+Math.round(maxwl*100)/100+" inches</td>"+
                                                    "</tr>"+
                                                    "<tr>"+
                                                        "<td align=\"right\"><b>Minimum water level: </b></td><td>"+Math.round(minwl*100)/100+" inches</td>"+
                                                    "</tr>"+
                                                 "</table>";


    //...Show last 24 hrs
    var xmax = floatData[floatData.length-1][0];
    var xmin = Math.max(floatData[0][0],floatData[floatData.length-1][0]-(24*60*60*1000));


    //var xData = { min: xmin, max: xmax, type: 'datetime', title: { text: 'Date (Mountain Time)' }, dateTimeLabelFormats: { month: '%e. %b', year: '%b' }, gridLineWidth: 1 };
    var xData = { type: 'datetime', title: { text: 'Date (Mountain Time)' }, dateTimeLabelFormats: { month: '%e. %b', year: '%b' }, gridLineWidth: 1 };
    var plotOption = { series: { marker: { enabled: false }, animation: { enabled: true, duration: 1000, easing: 'linear' }, } };
    var plotToolTip = {
             formatter: function() {
                 return '<b>'+ this.series.name +'</b><br/>'+
                 Highcharts.dateFormat('%b %e, %Y %l:%M%p', this.x) +': '+ this.y.toFixed(2) +' ';
             }
    };


    $('#waterlevel').highcharts({
        title: {
            text: 'Sump Water Level',
            x: -20 //center
        },
        chart : { zoomType: "xy" },
        xAxis: xData,
        yAxis: { labels: {format: '{value:.2f}'}, title: { text: 'Water Level (in)' }, gridLineWidth: 1, alternateGridColor: '#EEEEEE' },
        series: [{ name: "Water Level", data: waterData, color: '#0000FF' }],
        plotOptions: { line: { marker: { enabled: false }}}
    });
    

}