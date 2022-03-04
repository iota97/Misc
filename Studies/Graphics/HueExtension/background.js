document.addEventListener("DOMContentLoaded", () => {
    var slider = document.getElementById("Extension-hueSlider___")
    slider.value = localStorage["hueValue"];    
    chrome.tabs.executeScript(null, {code: "document.documentElement.style.setProperty('filter', 'hue-rotate("+slider.value+"deg)');"});

    slider.addEventListener("input", (e) => {
        localStorage["hueValue"] = slider.value;
        chrome.tabs.executeScript(null, {code: "document.documentElement.style.setProperty('filter', 'hue-rotate("+slider.value+"deg)');"});
    })
})