setTimeout(function () {
    location.reload();
}, 10000);
$(document).ready(function () {
    $.getJSON('https://api.thingspeak.com/channels/434997/feed/last.json?api_key=P6NUL7CSV20DGH2D', function (jsonData) {
        var pressure = parseFloat(jsonData.field1);
        var temperature = parseFloat(jsonData.field2);
        var state = parseInt(jsonData.field3);
        var pLimit = parseFloat(jsonData.field4);
        var tLimit = parseFloat(jsonData.field5);
        /*Temperature Gauge Chart*/
        new Chartist.Pie('.ct-chart', {
            series: [temperature, 100 - temperature]
        }, {
            donut: true,
            donutWidth: 60,
            startAngle: 270,
            total: 200,
            showLabel: true
        });
        /*Compressor State*/
        if (state === 1) {
            $('#state').append('OFF <i class="material-icons red">radio_button_checked</i>');
        } else if (state === 0) {
            $('#state').append('ON <i class="material-icons">radio_button_checked</i>');
        }
        /*Storage Tank Capacity*/
        var capacity = parseFloat(pressure / 4) * 100;
        $('#capacity').append(capacity + '%');
        /*Set Points*/
        $('#pLimit').append(pLimit + ' Bar');
        $('#tLimit').append(tLimit + '&deg;C');
        /*Alarms*/
        if (pressure >= pLimit) {
            $('#pAlarm').append('<i class="material-icons red">radio_button_checked</i>');
        } else {
            $('#pAlarm').append('<i class="material-icons">radio_button_checked</i>');
        }
        if (temperature >= tLimit) {
            $('#tAlarm').append('<i class="material-icons red">radio_button_checked</i>');
        } else {
            $('#tAlarm').append('<i class="material-icons">radio_button_checked</i>');
        }
    });
    /*Calculate Average Pressure*/
    $.getJSON('https://api.thingspeak.com/channels/434997/fields/1.json?results=10', function (jsonData) {
        var avg = 0;
        for (var i = 0; i < 10; i++) {
            avg = avg + parseFloat(jsonData.feeds[i].field1);
        }
        avg = avg/10;
        avg = avg.toFixed(2);
        $('#pAvg').append(avg + ' Bar');
    });
    /*Calculate Average Temperature*/
    $.getJSON('https://api.thingspeak.com/channels/434997/fields/2.json?results=10', function (jsonData) {
        var avg = 0;
        for (var i = 0; i < 10; i++) {
            avg = avg + parseFloat(jsonData.feeds[i].field2);
        }
        avg = avg/10;
        avg = avg.toFixed(2);
        $('#tAvg').append(avg + '&deg;C');
    });
});