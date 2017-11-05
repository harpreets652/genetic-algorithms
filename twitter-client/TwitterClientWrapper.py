import twitter as tweet
import os

PROPERTIES_FILE = 'twitter-client.properties'
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

