
function switchIcon(iconid) {
    var mark = "toolbar-favorite-mark";
    var unmark = "toolbar-favorite-unmark";
    if (iconid === unmark) {
        return mark
    } else {
        return unmark
    }
}

function loadPage(file, param) {
    var component = Qt.createComponent(file)
    if (component.status === Component.Ready) {
        if (param !== undefined) {
            pageStack.push(component, param);
        } else {
            pageStack.push(component);
        }
    } else {
        console.log("Error loading: " + component.errorString());
    }
}

function loadObject(parent, file, properties) {
    console.log("loading", file)
    var component = Qt.createComponent(file)
    console.log("loaded", file)

    if (component.status === Component.Ready) {
        console.log("create", file)
        var comp = component.createObject(parent, properties)
        console.log("created")
        return comp
    } else {
        console.log("Error loading dialog:", component.errorString());
    }
}

