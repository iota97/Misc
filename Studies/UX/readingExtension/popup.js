document.addEventListener("DOMContentLoaded", () => {
    apply = () => {
        chrome.tabs.executeScript(null, { code:
            `document.querySelectorAll("p, p *")
            .forEach(elem =>
                elem.innerHTML = elem.innerText
                .split(" ")
                .map(word =>
                    "<b>"+word.substring(0, Math.ceil(word.length/2))+"</b>"+word.substring(Math.ceil(word.length/2))
                    )
                    .join(" ")
                    )`
        })
    }
   
    document.getElementById("__apply").addEventListener("click", apply)
})