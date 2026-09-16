const mode = document.querySelector("#mode");
const currentKey = document.querySelector("#current-key");
const previousKey = document.querySelector("#previous-key");
const rotateForm = document.querySelector("#rotate-form");
const newKey = document.querySelector("#new-key");
const endTransitionAction = document.querySelector("#end-transition-action");
const endTransitionButton = document.querySelector("#end-transition");
const actionError = document.querySelector("#action-error");
const requestLog = document.querySelector("#request-log");
const streamStatus = document.querySelector("#stream-status");
const clearLogButton = document.querySelector("#clear-log");

const categoryLabels = {
  public: "Page publique",
  "current-key": "Clé principale",
  "previous-key-transition": "Ancienne clé (transition)",
  "unknown-key": "Clé inconnue",
  "missing-key": "Aucune clé",
};

function showError(message = "") {
  actionError.hidden = !message;
  actionError.textContent = message;
}

function renderState(state) {
  mode.textContent = state.mode === "transition" ? "Transition" : "Normal";
  mode.className = `mode ${state.mode}`;
  currentKey.textContent = state.current_key;
  previousKey.textContent = state.previous_key ?? "Non définie";
  previousKey.classList.toggle("undefined-key", state.previous_key === null);
  rotateForm.hidden = state.mode !== "normal";
  endTransitionAction.hidden = state.mode !== "transition";
}

async function requestJson(url, options) {
  const response = await fetch(url, options);
  const data = await response.json();
  if (!response.ok) {
    throw new Error(data.error ?? "La requête a échoué.");
  }
  return data;
}

async function loadState() {
  try {
    renderState(await requestJson("/admin/api/state"));
  } catch (error) {
    showError(error.message);
  }
}

rotateForm.addEventListener("submit", async (event) => {
  event.preventDefault();
  showError();
  try {
    const state = await requestJson("/admin/api/rotate", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ new_key: newKey.value }),
    });
    newKey.value = "";
    renderState(state);
  } catch (error) {
    showError(error.message);
  }
});

endTransitionButton.addEventListener("click", async () => {
  showError();
  try {
    renderState(await requestJson("/admin/api/end-transition", { method: "POST" }));
  } catch (error) {
    showError(error.message);
  }
});

function prependLog(entry) {
  const item = document.createElement("li");
  item.className = entry.category;
  const time = new Date(entry.timestamp).toLocaleTimeString();
  item.textContent = `${time}  ${entry.method} ${entry.path}  |  ${categoryLabels[entry.category]}`;
  requestLog.prepend(item);
}

let stream;

function connectStream() {
  stream = new EventSource("/admin/api/logs/stream");
  stream.addEventListener("open", () => {
    streamStatus.textContent = "SSE connecté";
    streamStatus.className = "stream-status connected";
  });
  stream.addEventListener("error", () => {
    streamStatus.textContent = "SSE en reconnexion…";
    streamStatus.className = "stream-status disconnected";
  });
  stream.addEventListener("request-log", (event) => prependLog(JSON.parse(event.data)));
}

clearLogButton.addEventListener("click", async () => {
  stream.close();
  clearLogButton.disabled = true;
  try {
    await requestJson("/admin/api/logs/clear", { method: "POST" });
    requestLog.replaceChildren();
  } catch (error) {
    showError(error.message);
  } finally {
    clearLogButton.disabled = false;
    connectStream();
  }
});

connectStream();
loadState();
