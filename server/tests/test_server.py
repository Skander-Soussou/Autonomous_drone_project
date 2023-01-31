from flask import Flask, request, json;
import pytest
import os
import sys
sys.path.append(os.path.abspath("../"))
import server as s;


def test_type_mission():
    response = s.app.test_client().get('/state'); 
    print(response.data)
    assert bool(response.data) == True; 



    
    