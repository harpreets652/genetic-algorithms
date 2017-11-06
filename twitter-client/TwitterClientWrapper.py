import twitter
import twitter.models
import os

PROPERTIES_FILE = 'twitter-client.properties'
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

properties = dict(line.strip().split('=') for line in open(ROOT_DIR + '/' + PROPERTIES_FILE))

api = twitter.Api(consumer_key=properties.get("api-key"),
                  consumer_secret=properties.get("api-secret"),
                  access_token_key=properties.get("access-token-key"),
                  access_token_secret=properties.get("access-token-secret"))

# users = api.UsersLookup(screen_name='CS_776_GA')
users = api.GetFriends()
trends = api.GetTrendsCurrent()

print([u.screen_name for u in users])
print (trends)