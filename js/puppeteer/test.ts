import { default as puppeteer, Page, ElementHandle } from "puppeteer";

const DEFAULT_TIMEOUT = 3000;

async function sleep(milliseconds: number): Promise<void> {
  await new Promise(resolve => setTimeout(resolve, milliseconds));
}

async function wait_for_selector_or_throw(page: Page, selector: string, options: object): Promise<ElementHandle<Element>> {
  // prevent the caller to check for null, cf. https://github.com/puppeteer/puppeteer/issues/3877
  const item = await page.waitForSelector(selector, options);
  if (item === null) { throw Error(`wait_for_element is null for '${selector}'`); }
  return item;
}


async function wait_for_single_selector(page: Page, selector: string, timeout = DEFAULT_TIMEOUT): Promise<ElementHandle<Element>> {
  // precondition = the element exists and is unique
  const element = await wait_for_selector_or_throw(page, selector, {timeout: timeout, visible: true});
  const elements = await page.$$(selector);
  if (elements.length == 0) { throw Error(`empty elements for selector '${selector}'`); }
  if (elements.length > 1) { throw Error(`there are more than one element (${elements.length}) for selector ${selector}`); }
  return element;
}

async function get_text_content(page: Page, selector: string): Promise<string> {
  // precondition = the selector exists
  const text =  await page.$eval(selector, el => el.textContent);
  if (text === null) {
    throw Error(`unable to get text for selector ${selector}`);
  }
  return text;
}

function assert(condition: boolean) {
  if (!condition) { throw Error("ASSERT FAILED"); }
}

async function main() {
  // parse host :
  const args = process.argv.slice(2);
  const DEFAULT_URL = "http://localhost:5173";  // vite's local dev server
  const host = args[0] !== undefined ? args[0] : DEFAULT_URL;
  console.log(`Testing host ${host}`);

  // create browser + get page :
  const headless = "new";  // <---------- change this to false to visualize test while it's live-running
  const browser = await puppeteer.launch({
    headless: headless,
    args: [`--window-size=1024,768`],
    defaultViewport: {
      width: 1024,
      height: 768
    }
  });
  const page = (await browser.pages()).at(0);
  if (page === undefined) { throw Error("page is undefined"); }

  const url = `${host}/index.html`;
  await page.goto(url);

  // check initial state :
  await wait_for_single_selector(page, "#day");
  const day1 = await get_text_content(page, "#day");
  assert(day1 == "LUNDI");

  // click button and wait for second description :
  const button = await wait_for_single_selector(page, "#button");
  await button.click();
  await wait_for_single_selector(page, "#description2");

  // wait for some more time, then check that day changed :
  await sleep(1000);
  const day2 = await get_text_content(page, "#day");
  assert(day2 == "MARDI");

  console.log("SUCCESS, YAY !");
  await browser.close();
}

main();
