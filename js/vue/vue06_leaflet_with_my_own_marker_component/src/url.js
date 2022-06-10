function get_int_param(params, key) {
    if (params.get(key) === null) {
            throw `inexisting param's key : ${key}`;
    }
    const parsed_int = parseInt(params.get(key), 10);
    if (isNaN(parsed_int)) {
        throw `unable to parse int param : ${params.get(key)}`;
    }
    return  parsed_int;
}

function get_latlng_param(params, key) {
    if (params.get(key) === null) {
            throw `inexisting param's key : ${key}`;
    }
    const [lat_str,lng_str] = params.get(key).split(",");
    const lat = parseFloat(lat_str);
    const lng = parseFloat(lng_str);
    if (isNaN(lat) || isNaN(lng)) {
        throw `unable to parse coords param : ${params.get(key)}`;
    }
    return [lat, lng];
}

export function parse_url_params(url, default_center, default_zoom) {
    let center = default_center;
    let zoom = default_zoom;
    let markerpos = null;
    let params = (new URL(url)).searchParams;
    try { zoom   = get_int_param(   params, "mapzoom");   } catch(error) { console.log(error); }
    try { center = get_latlng_param(params, "mapcenter"); } catch(error) { console.log(error); }
    try { markerpos = get_latlng_param(params, "markerpos"); } catch(error) { console.log(error); }
    return [center, zoom, markerpos];
}

export function parse_current_url_params(default_center, default_zoom) {
    return parse_url_params(document.location, default_center, default_zoom);
}

export function get_updated_url(updated_params) {
    let url = new URL(window.location.href);
    let params = new URLSearchParams(url.search);
    for (const name in updated_params) {
        params.set(name, updated_params[name]);
    }
    url.search = params.toString();
    return url;
}

export function update_current_url(updated_params) {
    let new_url = update_url(upated_params);
    // update URL without reloading page :
    window.history.replaceState({}, "", url);
}
