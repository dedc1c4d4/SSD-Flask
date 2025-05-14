from flask import Flask, render_template, request, redirect
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///firstapp.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

class Student(db.Model):
    sno = db.Column(db.Integer, primary_key=True)
    fname = db.Column(db.String(100))
    lname = db.Column(db.String(100))
    email = db.Column(db.String(100))
    phone = db.Column(db.String(100))

    def __repr__(self):
        return f"{self.sno} - {self.fname}"

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        fname = request.form['fname']
        lname = request.form['lname']
        email = request.form['email']
        phone = request.form['phone']
        student = Student(fname=fname, lname=lname, email=email, phone=phone)
        db.session.add(student)
        db.session.commit()
        return redirect('/')
    all_students = Student.query.all()
    return render_template('index.html', students=all_students)

@app.route('/delete/<int:sno>')
def delete(sno):
    student = Student.query.get_or_404(sno)
    db.session.delete(student)
    db.session.commit()
    return redirect('/')

@app.route('/update/<int:sno>', methods=['GET', 'POST'])
def update(sno):
    student = Student.query.get_or_404(sno)
    if request.method == 'POST':
        student.fname = request.form['fname']
        student.lname = request.form['lname']
        student.email = request.form['email']
        student.phone = request.form['phone']
        db.session.commit()
        return redirect('/')
    return render_template('update.html', student=student)

if __name__ == '__main__':
    app.run(debug=True)