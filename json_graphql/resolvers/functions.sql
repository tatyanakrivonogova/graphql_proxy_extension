getPerson='SELECT json_agg(person) AS person FROM (SELECT person.id, person.name, person.age FROM graphql_proxy.Person WHERE person.id = $1) AS person;';

getMessage='SELECT json_agg(message) AS message FROM (SELECT message.id, message.content FROM graphql_proxy.Message INNER JOIN graphql_proxy.Person ON message.author = Person.pk_Person WHERE message.id = $1) AS message;';

createPerson='INSERT INTO graphql_proxy.Person(id, name, age) VALUES($1, $2, $3);';

updatePerson='UPDATE graphql_proxy.Person SET name = $1, age = $2 WHERE id = $3';

deletePerson='DELETE FROM graphql_proxy.Person WHERE id = $1;';

createMessage='INSERT INTO graphql_proxy.message (id, content, author) VALUES ($1, $2, (SELECT pk_Person FROM graphql_proxy.person WHERE id = $3));';