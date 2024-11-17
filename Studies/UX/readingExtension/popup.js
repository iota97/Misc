document.addEventListener("DOMContentLoaded", () => {
    apply = () => {
        chrome.tabs.executeScript(null, { code:
            `(() => {
                const process = (element, callback) => {
                    for (const childNode of element.childNodes) {
                        childNode.nodeType == Node.TEXT_NODE ? (element.nodeName == 'SCRIPT' || callback(childNode)) : process(childNode, callback)
                    }
                }
                process(document.body, element => {
                    wrapper = document.createElement('span')
                    element.parentNode.insertBefore(wrapper, element);
                    wrapper.appendChild(element);
                    wrapper.innerHTML = element.data
                        .split(" ")
                        .map(word => "<b>"+word.substring(0, Math.ceil(word.length/2))+"</b>"+word.substring(Math.ceil(word.length/2)))
                        .join(" ")
                })
            })()`
        })
    }
   
    document.getElementById("__apply").addEventListener("click", apply)
})