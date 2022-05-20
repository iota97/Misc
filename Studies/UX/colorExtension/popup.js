document.addEventListener("DOMContentLoaded", () => {
    var hue = document.getElementById("Extension-hueSlider___")
    var sat = document.getElementById("Extension-satSlider___")
    var bri = document.getElementById("Extension-briSlider___")
    var con = document.getElementById("Extension-conSlider___")
    var res = document.getElementById("Extension-reset___")

    var outhue = document.getElementById("huedisplay")
    var outsat = document.getElementById("satdisplay")
    var outbri = document.getElementById("bridisplay")
    var outcon = document.getElementById("condisplay")

    hue.value = localStorage["hueValue"] || 0
    sat.value = localStorage["satValue"] || 100
    bri.value = localStorage["briValue"] || 100
    con.value = localStorage["conValue"] || 100

    const apply = (e) => {
        localStorage["hueValue"] = hue.value
        localStorage["satValue"] = sat.value
        localStorage["briValue"] = bri.value
        localStorage["conValue"] = con.value

        outhue.innerHTML = hue.value + "&deg;"
        outsat.innerHTML = sat.value + "&percnt;"
        outbri.innerHTML = bri.value + "&percnt;"
        outcon.innerHTML = con.value + "&percnt;"

        chrome.tabs.executeScript(null, {code: 
            "document.documentElement.style.setProperty('filter', 'hue-rotate("+hue.value+"deg) saturate("+sat.value+"%) contrast("+con.value+"%) brightness("+bri.value+"%)');"
        }) 
    }
    apply()

    hue.addEventListener("input", apply)
    sat.addEventListener("input", apply)
    bri.addEventListener("input", apply)
    con.addEventListener("input", apply)

    res.addEventListener("click", (e) => {
        hue.value = 0
        sat.value = 100
        bri.value = 100
        con.value = 100
        apply();
    })
})