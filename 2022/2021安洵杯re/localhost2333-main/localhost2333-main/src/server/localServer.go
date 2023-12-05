package server

import (
	"html/template"
	"log"
	"net/http"
	"vm"
)

func getFlag(w http.ResponseWriter, r *http.Request) {

	var data struct{
		Str template.HTML
	}
	const result = `<div style="text-align:center;vertical-align:middel;">
                 <p>{{.Str}}</p>
                 </div>`

	const templ = `<!DOCTYPE html>
					<html lang="en">
					<head>
						<meta charset="UTF-8">
						<title>localhost:2333</title>
					</head>
					<body>
						<form method="post">
							<div style="text-align:center;vertical-align:middel;">
							flag: <input type="text" name="flag">
							<input type="submit" value="Submit">
							</div>
						</form>
					</body>
					</html>`

	t := template.Must(template.New("input").Parse(templ))
	if err := t.Execute(w, nil); err != nil {
		log.Fatal(err)
	}


	flag := r.PostFormValue("flag")

	if len(flag) == 30{
		data.Str = vm.MainVM(flag)
	} else {
		data.Str = "Length error!"
	}

	ret := template.Must(template.New("result").Parse(result))
	if err := ret.Execute(w, data); err != nil {
		log.Fatal(err)
	}
}


func Server() {
	http.HandleFunc("/", getFlag)
	log.Fatal(http.ListenAndServe("localhost:2333", nil))
}
