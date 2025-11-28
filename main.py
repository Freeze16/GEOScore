from fastapi import FastAPI, HTTPException
from fastapi.staticfiles import StaticFiles
from logic import HtmlParser
import requests

import os
from dotenv import load_dotenv
from llm_utils.groq_prompt import prompt_groq

load_dotenv()

app = FastAPI(title="GEOScore", version="0.2.0")

api_key = os.getenv('API_KEY')


@app.post("/api/analyze")
async def analyze_url(request: str):
    try:
        html = requests.get(request)
    except:
        raise HTTPException(status_code=400, detail="Url is not valid")

    if not (html.status_code == 200 and html.headers.get('content-type').startswith('text/html')):
        raise HTTPException(status_code=400, detail="Page not found or not HTML")

    parser = HtmlParser(html.content)
    result = prompt_groq(api_key, parser)

    return {"result": result}


app.mount("/", StaticFiles(directory="static", html=True), name="static")

if __name__ == "__main__":
    import uvicorn

    uvicorn.run(app, host="0.0.0.0", port=8000)
