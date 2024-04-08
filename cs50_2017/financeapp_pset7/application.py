"""
IGNORE -- BOILERPLATE


from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")
"""

@app.route("/")
@login_required
def index():
    info = db.execute("SELECT symbol, shares FROM holdings WHERE userid = :uid", uid=session["user_id"])
    for i in info:
        curprice = lookup(i["symbol"])["price"]
        i.update({"price": usd(curprice)})
        i.update({"totalvalue": usd(i["shares"] * curprice)})
    return render_template("index.html", info=info, time=str(datetime.utcnow())[:19], cash=usd(db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]["cash"]))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    cash = db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]["cash"]
    if request.method == "GET":
        return render_template("buy.html",cash=usd(cash))
    elif request.method == "POST":
        info = lookup(request.form.get("symbol").upper())
        if info == None or info["price"] < 0.01:
            return render_template("buy.html", show=0,cash=usd(cash))
        info["price"] = round(info["price"], 2)
        info.update({"sharenum": int(request.form.get("sharenum"))})
        if cash < (info["sharenum"] * info["price"]):
            return render_template("buy.html", show=1,cash=usd(cash))

        before = db.execute("SELECT * FROM holdings WHERE userid = :uid AND symbol = :sym", uid=session["user_id"], sym=info["symbol"])
        db.execute("INSERT INTO transactions (userid, symbol, stkprice, shares) VALUES (:uid, :sym, :pr, :sh)", uid=session["user_id"], sym=info["symbol"], pr=info["price"], sh=info["sharenum"])
        db.execute("UPDATE users SET cash = :nc WHERE id = :uid", nc=cash-info["sharenum"] * info["price"], uid=session["user_id"])
        db.execute("INSERT OR REPLACE INTO holdings (userid, symbol, shares) VALUES (:uid, :sym, :sh)", uid=session["user_id"], sym=info["symbol"], sh=before[0]["shares"] + info["sharenum"] if (len(before) > 0) else info["sharenum"])
        return render_template("buy.html", show=info,cash=usd(db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]["cash"]))
        
        
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    info = db.execute("SELECT symbol, stkprice, shares, time FROM transactions WHERE userid = :uid ORDER BY time DESC", uid = session["user_id"])
    for i in info:
        i["stkprice"] = usd(i["stkprice"])
    return render_template("history.html", info=info)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("NO SUCH USER")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""
    
    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    elif request.method == "POST":
        info = lookup(request.form.get("quote"))
        if info == None:
            return render_template("quote.html", htmlinfo=0)
        info["price"] = usd(info["price"])
        return render_template("quote.html", htmlinfo=info)
        
        
@app.route("/register", methods=["GET", "POST"])
def register():
    # Use python regex
    """Register user."""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        password = request.form.get("password")
        if request.form.get("confirmpassword") == password:
            session["user_id"] = db.execute("INSERT INTO users(username, hash) VALUES(:passusername, :phash)", 
            passusername = request.form.get("username"), phash = pwd_context.encrypt(password))
            return redirect(url_for("index"))
        else:
            return apology("Passwords...", "DO NOT MATCH!")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    cash = db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]["cash"]
    if request.method == "GET":
        return render_template("sell.html", cash=usd(cash))
    elif request.method == "POST":
        info = lookup(request.form.get("symbol").upper())
        if info == None or info["price"] < 0.01:
            return render_template("sell.html", show=0,cash=usd(cash))
        if {"symbol": info["symbol"]} not in db.execute("SELECT symbol FROM holdings WHERE userid = :uid AND symbol = :sym", uid=session["user_id"], sym=request.form.get("symbol").upper()):
            return render_template("sell.html", show=info["symbol"],cash=usd(cash))
        before = db.execute("SELECT * FROM holdings WHERE userid = :uid AND symbol = :sym", uid=session["user_id"], sym=info["symbol"])
        if int(request.form.get("sharenum")) > before[0]["shares"]:
            return render_template("sell.html", show=1,cash=usd(cash))
        info["price"] = round(info["price"], 2)
        info.update({"sharenum": int(request.form.get("sharenum"))})
        
        db.execute("INSERT INTO transactions (userid, symbol, stkprice, shares) VALUES (:uid, :sym, :pr, :sh)", uid=session["user_id"], sym=info["symbol"], pr=info["price"], sh=-info["sharenum"])
        db.execute("UPDATE users SET cash = :nc WHERE id = :uid", nc=cash+info["sharenum"] * info["price"], uid=session["user_id"])
        if info["sharenum"] == int(before[0]["shares"]):
            db.execute("DELETE FROM holdings WHERE userid = :uid AND symbol = :sym", uid=session["user_id"], sym=info["symbol"])
        else:
            db.execute("UPDATE holdings SET shares = :sh WHERE userid = :uid AND symbol = :sym", sh=before[0]["shares"] - info["sharenum"], uid=session["user_id"], sym=info["symbol"])
        return render_template("sell.html", show=info,cash=usd(db.execute("SELECT cash FROM users WHERE id = :uid", uid=session["user_id"])[0]["cash"]))
        
@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    """Displays user settings"""
    if request.method == "GET":
        return render_template("settings.html")
    elif request.method == "POST":
        if request.form["btn"] == "s1":
            password = request.form.get("password")
            if request.form.get("confirmpassword") == password:
                db.execute("UPDATE users SET hash = :phash WHERE id = :uid", phash=pwd_context.encrypt(password), uid=session["user_id"])
                return render_template("settings.html", info=0)
            return render_template("settings.html", info=1)
        elif request.form["btn"] == "s2":
            if request.form.get("rly") == "Yes" and request.form.get("rlyrly") == "Really Sure" and request.form.get("rlyrlyrly") == "REALLY REALLY SURE":
                db.execute("DELETE FROM transactions WHERE userid = :uid", uid=session["user_id"])
                db.execute("DELETE FROM holdings WHERE userid = :uid", uid=session["user_id"])
                db.execute("DELETE FROM users WHERE id = :uid", uid=session["user_id"])
                session.clear()
                return redirect(url_for("login"))
            return render_template("settings.html", info=2)