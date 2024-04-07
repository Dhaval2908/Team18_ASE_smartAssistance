from flask import Flask, render_template, request
import pickle

app = Flask(__name__,static_folder='static')

# Load the models from Pickle files
with open("smartttdata_alert_model.pkl", "rb") as f:
    alert_model = pickle.load(f)


@app.route("/")
def index():
    return render_template("index_four.html")

@app.route("/predict", methods=["POST"])
def predict():
    # Retrieve form data
    age = int(request.form["age"])
    
    trestbps = float(request.form["trestbps"])
    
    chol = float(request.form["chol"])
    
    thalch = float(request.form["thalch"])
    
    sex=int(request.form['sex'])
    if(sex==1):
            sex_Female=0
            sex_Male=1
    else:                            
            sex_Female=1
            sex_Male=0
            
    cp=int(request.form['cp'])
    if(cp==0):
            cp_asymptomatic=1
            cp_atypical_angina=0
            cp_non_anginal=0
            cp_typical_angina=0       
    elif(cp==1):                            
            cp_asymptomatic=0
            cp_atypical_angina=1
            cp_non_anginal=0
            cp_typical_angina=0
    elif(cp==2):                            
            cp_asymptomatic=0
            cp_atypical_angina=0
            cp_non_anginal=1
            cp_typical_angina=0
    else:
            cp_asymptomatic=0
            cp_atypical_angina=0
            cp_non_anginal=0
            cp_typical_angina=1

    # Make predictions using the loaded models
    alert_prediction = alert_model.predict([[age,trestbps,chol,thalch,
                                             sex_Female,sex_Male,
                                             cp_asymptomatic,cp_atypical_angina,
                                             cp_non_anginal,cp_typical_angina]])[0]

    return render_template("index_four.html", alert=alert_prediction)

if __name__ == "__main__":
    app.run(debug=True)
