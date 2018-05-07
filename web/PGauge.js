var data, chart, pressure;

google.charts.load('current', {'packages': ['gauge']});

google.setOnLoadCallback(loadChart);

function displayChart(point, name) {
    data.setValue(0, 0, name);
    data.setValue(0, 1, point);
    chart.draw(data, options);
}

function loadData() {
    $.getJSON('https://api.thingspeak.com/channels/434997/feed/last.json?api_key=P6NUL7CSV20DGH2D', function (jsonData) {
        pressure = jsonData.field1;
        displayChart(pressure, 'Pressure');
    });
}

function loadChart() {
    data = new google.visualization.DataTable();
    data.addColumn('string', 'Label');
    data.addColumn('number', 'Value');
    data.addRows(1);

    chart = new google.visualization.Gauge(document.getElementById('pGauge'));
    options = {min: 0, max: 10, redFrom: 9, redTo: 10, yellowFrom: 7.5, yellowTo: 9, minorTicks: 10};

    loadData();

    setInterval('loadData()', 15000);
}