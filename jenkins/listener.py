#!/usr/bin/env python2.7

PORT = 41372
DEVICE = '/dev/ttyACM0'

from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import json
import serial

status = {
	'SUCCESS': 'S',
	'UNSTABLE': 'U',
	'FAILURE': 'F',
}

class JenkinsHandler(BaseHTTPRequestHandler):
	def do_POST(self):
		length = int(self.headers['Content-length'])
		data = self.rfile.read(length)
		
		notification = json.loads(data)['build']
		
		if notification['phase'] == 'STARTED':
			light.write('0')
		elif notification['phase'] == 'FINISHED':
			light.write(status[notification['status']])
		
		self.send_response(200)
		self.send_header("Content-Length", 0)
		self.end_headers()

light = serial.Serial(DEVICE, 9600)
HTTPServer(('0.0.0.0', PORT), JenkinsHandler).serve_forever()