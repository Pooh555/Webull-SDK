package webull.trading.bot.user;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.PropertyNamingStrategies;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.InputStream;
import java.io.IOException;

public class User {
    private static final Logger logger = LoggerFactory.getLogger(User.class);

    private String username = "default";
    private String id       = "";
    private String api_key  = "";
    private String secret   = "";

    public User() {}

    public User(String username, String secret_path) {
        this.username = username;
        load_secrets(secret_path);

        logger.info("[User] Successfully initialized user: {}", this.username);
    }

    private void load_secrets(String secret_path) {
        ObjectMapper mapper = new ObjectMapper();
        mapper.setPropertyNamingStrategy(PropertyNamingStrategies.SNAKE_CASE);

        try (InputStream input_stream = getClass().getClassLoader().getResourceAsStream(secret_path)) {
            if (input_stream == null) {
                logger.error("[User] Could not find resource file: {}", secret_path);
                return;
            }

            User loaded_user = mapper.readValue(input_stream, User.class);
            this.id      = loaded_user.get_id();
            this.api_key = loaded_user.get_api_key();
            this.secret  = loaded_user.get_secret();
            
            logger.info("[User] Successfully loaded secrets for user: {}", this.username);
        } catch (IOException e) {
            logger.error("[User] Failed to parse secrets from resource {}", secret_path, e);
        }
    }

    public String get_username() {
        return username;
    }

    public void set_username(String username) {
        this.username = username;
    }

    public String get_id() {
        return id;
    }

    public void set_id(String id) {
        this.id = id;
    }

    public String get_api_key() {
        return api_key;
    }

    public void set_api_key(String api_key) {
        this.api_key = api_key;
    }

    public String get_secret() {
        return secret;
    }

    public void set_secret(String secret) {
        this.secret = secret;
    }
}