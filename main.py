from fastapi import FastAPI, HTTPException
from fastapi.staticfiles import StaticFiles
from logic import HtmlParser
import requests

app = FastAPI(title="GEOScore", version="0.1.0")

@app.post("/api/analyze")
async def analyze_url(request: str):
    try:
        html = requests.get(request)
    except:
        raise HTTPException(status_code=400, detail="Url is not valid")
    
    if not (html.status_code == 200 and html.headers.get('content-type').startswith('text/html')):
        raise HTTPException(status_code=400, detail="Page not found or not HTML")

    parser = HtmlParser(html.content)
    print(parser.title, parser.author, parser.description)

    return {"result": 1.0}

app.mount("/", StaticFiles(directory="static", html=True), name="static")

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
