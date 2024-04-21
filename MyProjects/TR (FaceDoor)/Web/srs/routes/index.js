const { Router } = require('express');
var userId = ''
var nom = ''
var am=0
var kt=''
var firebase = require("firebase");
const router = Router();
const admin = require('firebase-admin');
var serviceAccount = require('../../reconeixement-facial-firebase-adminsdk-3mi9b-f58cf111f8.json');
const firebaseConfig = {
    apiKey: "AIzaSyBMkuJ7OKJn5XrLM46o_8sqW3TNyfFOU4Y",
    authDomain: "reconeixement-facial.firebaseapp.com",
    databaseURL: "https://reconeixement-facial-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "reconeixement-facial",
    storageBucket: "reconeixement-facial.appspot.com",
    messagingSenderId: "414222554752",
    appId: "1:414222554752:web:f6b256640750b69d32b865",
    measurementId: "G-20X4ED5LDX"
};

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount), databaseURL:'https://reconeixement-facial-default-rtdb.europe-west1.firebasedatabase.app/'
});


firebase.initializeApp(firebaseConfig);
const db = admin.database();
const RefDb = admin.database().ref('id');


router.get('/', (req, res) => {
    res.render('index');
});



router.post('/userdata', (req, res) => {// funcio que s'executa al premer el boto de registret
    const confirm = req.body.confirm_password // importa la contrasenya de confirmació 
    const password = req.body.password // importa la contrasenya
    const email = req.body.email // importa el coreu electronic

   
    if (password == confirm){// comprova si les contrasenyes son iguals
        
        const auth = firebase.auth(); //crida la funcio auth de firebase 
        const promise = auth.createUserWithEmailAndPassword(email,password);//Utilitza la funcio createUserWithEmailAndPassword per xifrar en un token el correu i la contrasenya i ho guarda a la base de dades.
        promise
            .catch(e => console.log(e.message)); // En cas que passes un error la funció catch evita que s'aturi l'execució del programa
    
        auth.onAuthStateChanged(firebaseUser => { // Comprova si s'ha enviat correctament el token i que no n’hi hagués un amb el mateix coreu
            if(firebaseUser){ 
                res.render('screan', { layout : 'principal'});//en cas que tot fos correcte ens redirigeix a la pagina principal
            }
        });
    }else{
       res.redirect('/');// en cas que les contrasenyes no siguin iguals torna a carregar la pagin 
    }
    
});



router.post('/log' , (req,res)=>{
    res.render('Login');
});

router.post('/log_in', (req,res)=>{
    const password= req.body.password
    const email = req.body.email
    const auth = firebase.auth();
    const promise = auth.signInWithEmailAndPassword(email,password);
    promise
        .then((a)=>{
            res.render('screan', { layout : 'principal'});
        })
        .catch((e)=>{
            console.log(e.message);
            res.render('Login');
        });
});

router.get('/inici', (req,res)=>{
    res.render('screan', {layout:'principal'});
});

router.get('/registre',(req,res)=>{
    if(userId!=''){
        RefDb.child(userId).child('white_list').once('value', (snap)=>{
            const data = snap.val();
            if(data==null){
                amount=0
            }else{
                amount=Object.keys(data).length;
            }
            res.render('registre',{users : data, layout:'principal', check: userId, userInfo: amount });
        });
    }else{
        res.render('registre',{layout:'principal', check: userId});
    }
   
    
});

router.get('/reconeixament',(req,res)=>{
    if(userId!=''){
        RefDb.child(userId).child('white_list').once('value', (snap)=>{
            const data = snap.val();
            res.render('reconeixament',{layout: 'principal', check: userId, users : data });
        });
    }else{
        res.render('reconeixament',{layout: 'principal', check: userId });
    }
    
});

router.get('/instruccions',(req,res)=>{
    res.render('instruccions',{layout:'principal'});
});

router.post('/reco',(req,res)=>{
    userId = req.body.id

    RefDb.child(userId).get().then((snapshot) => {
        if (snapshot.exists()) {
            RefDb.child(userId).child('white_list').once('value', (snap)=>{
                const data = snap.val();
                res.render('reconeixament',{users : data, layout:'principal', check: userId});
            });
        } else {
            res.render('reconeixament',{layout: 'principal'});
        }
      }).catch((error) => {
        console.error(error);
        });
});

router.post('/adduser',(req,res)=>{
    var storesRef = RefDb.child('lyaKyt4/white_list');

    const user = req.body.name
    RefDb.child(userId).child('white_list').orderByChild('name').equalTo(user).once("value",snapshot => {
        if (snapshot.exists()){
            RefDb.child(userId).child('white_list').once('value', (snap)=>{
                const data = snap.val();
                res.render('reconeixament',{users : data, layout:'principal', check: userId});
            });
        }else{
            var newStoreRef = storesRef.push();
            newStoreRef.set({
              name: req.body.name,
              status: true,
              number: 0
            });
            RefDb.child(userId).child('white_list').once('value', (snap)=>{
                const data = snap.val();
                console.log(data)
                res.render('reconeixament',{users : data, layout:'principal', check: userId});
            });
        }
    });
    

});
router.get('/re', (req,res)=>{
    RefDb.child('lyaKyt4').child('white_list').get().then((snapshot) => {
        if (snapshot.exists()) {
            RefDb.child('lyaKyt4').child('white_list').once('value', (snap)=>{
                const data = snap.val();
                console.log(data)
                res.render('reconeixament',{users : data, layout:'principal', check: userId});
            });
        } else {
            res.render('reconeixament',{layout:'principal', check: userId});
        }
    }).catch((error) => {
        console.error(error);
    });
});

router.get('/delete-contact/:id', (req,res)=>{
    console.log(req.params.id);
    RefDb.child('lyaKyt4/white_list/'+ req.params.id).remove();
    res.redirect('/re');
    
});
router.get('/regis',(req,res)=>{
    console.log(nom);
    var key=''
    dt=''
    //res.render('registre',{layout:'principal'});
    RefDb.child(userId).child('white_list').orderByChild('name').equalTo(nom).once("value",snapshot => {
        if (snapshot.exists()){
            d=snapshot.val();
            key=Object.keys(d)
            RefDb.child(userId).child('white_list').once('value', (snap)=>{
                const data = snap.val();
                
                am=Object.keys(data).length;
                dt=snap.val();
                kt=key[0]
                console.log(key[0],nom);
                var date = new Date();
                var day  = date.getDate()
                var year = date.getFullYear()
                var month = date.getMonth() + 1;
                if(month<10){
                    var d= day+'-'+ 0 + month + '-' + year
                }else{
                    var d= day+'-'+ month + '-' + year
                }
                RefDb.child( userId +'/white_list/' + key[0] + '/registre/' + d).get().then((st) => {
                    console.log(d)
                    console.log(key[0])
                    if (st.exists()) {
                        RefDb.child( userId +'/white_list/' + key[0] + '/registre/' + d).once('value', (sp)=>{
                            const d = sp.val();
                           res.render('registre',{layout:'principal', check: userId, userInfo: am, users: dt, temps: d, pressed : 1 });
                        });
                        
                    } else {
                        console.log('pinyi gay');
                        res.render('registre',{layout:'principal', check: userId, userInfo: am, users: dt, pressed : 1});
                    }
                }).catch((error) => {
                    console.error(error);
                });
            });
            
        }
    });

    
    
});

router.post('/calen',(req,res)=>{
    dataPerMoure=req.body.calendar
    var t = dataPerMoure.split('/')
    console.log(t)
    dataAreglada = t[1] +'-'+ t[0] + '-' + t[2]
    console.log('eyou gay',dataAreglada)
    RefDb.child( userId +'/white_list/' + kt + '/registre/' + dataAreglada).get().then((st) => {
        if (st.exists()) {
            RefDb.child( userId +'/white_list/' + kt + '/registre/' + dataAreglada).once('value', (sp)=>{
                const d = sp.val();
                console.log(d)
               res.render('registre',{layout:'principal', check: userId, userInfo: am, users: dt, temps: d, pressed : 1});
            });
            
        } else {
            console.log('pinyi gay');
            res.render('registre',{layout:'principal', check: userId, userInfo: am, users: dt, pressed : 1});
        }
    }).catch((error) => {
        console.error(error);
    });
});

router.get('/graphic/:id', (req,res)=>{
    nom=req.params.id
    res.redirect('/regis');
});
module.exports = router;