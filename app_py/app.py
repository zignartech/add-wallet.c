import subprocess
from flask import Flask, jsonify, request
app = Flask(__name__)

@app.route('/balance')
def getBalance():
    try:
        print("Total balance:")
        p = subprocess.Popen(["/home/pi/iota.c/build/examples/wallet_get_balance", "2aab5404c79c765593031fdfc4e30370815d8e90da19084015bc5c8eeba03e22"],
                     stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE
                    )

# Salida y errores
        out, err = p.communicate(timeout=10)
        print(out.decode("utf-8"))
# Estado de salida. Normalmente, su valor es 0
# si el programa ha terminado con éxito.
        exit_code = p.returncode        
        print(type(exit_code))
        if exit_code == 0:
            return jsonify({
               "error": False,
            "data": {
                "balance": out.decode("utf-8")
            }
        })
    except:
        return jsonify({
            "error": True,
            "message": "An error has occurred"
        })

@app.route('/address')
def listAddress():
    try:
        print("Get Address:")
        p = subprocess.Popen(["/home/pi/iota.c/build/examples/wallet_get_address", "2aab5404c79c765593031fdfc4e30370815d8e90da19084015bc5c8eeba03e22"],
                     stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE
                    )

# Salida y errores
        out, err = p.communicate(timeout=10)
        print(out.decode("utf-8"))
# Estado de salida. Normalmente, su valor es 0
# si el programa ha terminado con   xito.
        exit_code = p.returncode        
        print(exit_code)
        if exit_code == 0:
            return jsonify({
               "error": False,
            "data": {
                "Address": out.decode("utf-8").splitlines()[0]
            }
        })
    except:
        return jsonify({
            "error": True,
            "message": "An error has occurred"
        })

@app.route('/transactions')
def listTransfers():
    try:
        print("list-transactions:")
        p = subprocess.Popen(["/home/pi/iota.c/build/examples/wallet_get_msg", "2aab5404c79c765593031fdfc4e30370815d8e90da19084015bc5c8eeba03e22"],
                     stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE
                    )

# Salida y errores
        out, err = p.communicate(timeout=10)
        print(out.decode("utf-8"))
# Estado de salida. Normalmente, su valor es 0
# si el programa ha terminado con éxito.
        exit_code = p.returncode        
        print(type(exit_code))
        if exit_code == 0:
            return jsonify({
               "error": False,
            "data": {
                "transactions": out.decode("utf-8").splitlines()
            }
        })
    except:
        return jsonify({
            "error": True,
            "message": "An error has occurred"
        })

@app.route('/send-tokens', methods=['POST'])
def sendTokens():
    try:
        print(request.json)
        print("send-tokens:")
        p = subprocess.Popen(["/home/pi/iota.c/build/examples/wallet_send_tx", "2aab5404c79c765593031fdfc4e30370815d8e90da19084015bc5c8eeba03e22", "atoi1qrjj505wfzqt2n2wzpxe5vkjlktv3uvkemun8cz4eyc5utun26dlkvc6hek",request.json['cost'], f"""Device: {request.json['rover']['name']}
task: {request.json['rover']['tasks']}
location:  {request.json['rover']['location']}
cost: {request.json['cost']}
reference: {request.json['reference']}
farm_sections_involved: {request.json['farm_sections_involved']}
banner: {request.json['banner']}
dateGMT: {request.json['dateGMT']}"""],
                     stdout=subprocess.PIPE,
                     stderr=subprocess.PIPE
                    )

# Salida y errores
        out, err = p.communicate(timeout=15)
        print(out.decode("utf-8"))
# Estado de salida. Normalmente, su valor es 0
# si el programa ha terminado con   xito.
        exit_code = p.returncode        
        print(exit_code)
        if exit_code == 0:
            return jsonify({
               "error": False,
            "data": {
                "link": out.decode("utf-8")
            }
        })
    except Exception as e:
        print(e)
        return jsonify({
            "error": True,
            "message": "An error has occurred"
        })

if __name__ == '__main__':
    app.run(debug=True, port=4000, host="0.0.0.0")
