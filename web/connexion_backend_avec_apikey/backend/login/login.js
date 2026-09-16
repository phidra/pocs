const form = document.querySelector("#login-form");
const password = document.querySelector("#password");
const errorMessage = document.querySelector("#error");
const sessionCheck = document.querySelector("#session-check");

function getRedirectTarget() {
  const candidate = new URLSearchParams(window.location.search).get("redirect");
  if (!candidate || !candidate.startsWith("/") || candidate.startsWith("//")) {
    return "/board";
  }

  try {
    const target = new URL(candidate, window.location.origin);
    if (target.origin !== window.location.origin) {
      return "/board";
    }
    return `${target.pathname}${target.search}${target.hash}`;
  } catch {
    return "/board";
  }
}

const redirectTarget = getRedirectTarget();

function showError(message = "") {
  errorMessage.hidden = !message;
  errorMessage.textContent = message;
}

function showLoginForm() {
  sessionCheck.hidden = true;
  form.hidden = false;
  password.focus();
}

async function checkSession() {
  try {
    const response = await fetch("/api/session", { credentials: "same-origin" });
    if (response.ok) {
      window.location.replace(redirectTarget);
      return;
    }
    if (response.status === 401 || response.status === 403) {
      showLoginForm();
      return;
    }
    throw new Error("La vérification de session a échoué.");
  } catch (error) {
    showLoginForm();
    showError(error.message);
  }
}

form.addEventListener("submit", async (event) => {
  event.preventDefault();
  showError();

  try {
    const response = await fetch("/api/login", {
      method: "POST",
      credentials: "same-origin",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ password: password.value }),
    });
    if (response.ok) {
      window.location.assign(redirectTarget);
      return;
    }
    if (response.status === 401) {
      showError("Mot de passe incorrect.");
      return;
    }
    showError("La connexion a échoué.");
  } catch {
    showError("La connexion a échoué.");
  }
});

checkSession();
