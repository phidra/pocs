const value = document.querySelector("#value");
const togglePollingButton = document.querySelector("#toggle-polling");
const cookieValue = document.querySelector("#cookie-value");
let pollingId = null;
let redirectingToLogin = false;

function displayApiKeyCookie() {
  const prefix = "apikey=";
  const cookie = document.cookie.split("; ").find((entry) => entry.startsWith(prefix));
  cookieValue.textContent = cookie ? decodeURIComponent(cookie.slice(prefix.length)) : "Aucun cookie";
}

function redirectToLogin() {
  if (redirectingToLogin) {
    return;
  }

  redirectingToLogin = true;
  clearInterval(pollingId);
  pollingId = null;
  const requestedLocation = window.location.pathname + window.location.search + window.location.hash;
  window.location.replace(`/login?redirect=${encodeURIComponent(requestedLocation)}`);
}

async function refreshValue() {
  try {
    const response = await fetch("/api/current-value");
    if (response.status === 401 || response.status === 403) {
      redirectToLogin();
      return;
    }
    if (!response.ok) {
      throw new Error("Unable to load the current value.");
    }
    const data = await response.json();
    value.textContent = data.value;
    displayApiKeyCookie();
  } catch {
    value.textContent = "Indisponible";
  }
}

function startPolling() {
  refreshValue();
  pollingId = setInterval(refreshValue, 3000);
  togglePollingButton.textContent = "Mettre en pause";
  togglePollingButton.setAttribute("aria-pressed", "false");
}

function stopPolling() {
  clearInterval(pollingId);
  pollingId = null;
  togglePollingButton.textContent = "Reprendre le polling";
  togglePollingButton.setAttribute("aria-pressed", "true");
}

togglePollingButton.addEventListener("click", () => {
  if (pollingId === null) {
    startPolling();
  } else {
    stopPolling();
  }
});

displayApiKeyCookie();
startPolling();
